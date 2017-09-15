#ifndef READER_H
#define READER_H

#include "../include/Include.h"

#include "string"
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <stdlib.h>

#define LINE 6

class Reader
{
private:
    string pathFile;
    unordered_map<string, float> values;
//    vector<vector<float>> matrix;
    GLfloat** matrix;
    char *str;

    void createMatrix(bool incr) {
        int nrows = (int)(values["nrows"]);
        int ncols = (int)(values["ncols"]);
        if (incr) {
            nrows ++;
            ncols ++;
        }

        if (matrix == 0) {
            matrix = new float*[nrows];
            for (int j = 0; j < nrows; j++)
                matrix[j] = new float[ncols];
        }
    }

public:
    Reader(string pathFile) {
        this->pathFile = pathFile;
        this->matrix = 0;
        this->str = 0;
    }

    void loadData(bool incr) {
        int contLine = 0;
        int index_row = 0;

        string _string;
        ifstream infile;
        infile.open(pathFile);
        if (!infile) {
            cout<<"Unable to open file"<<endl;
            exit(1); // terminate with error
        }


//        char* str = "";
        char * pch = 0;

        while(!infile.eof()) // To get you all the lines.
        {
            getline(infile,_string); // Saves the line in _string.
            str = strdup(_string.c_str());

            if (contLine < LINE) {
                float value = 0.0;
                string key = "";
                int i = 0;


                pch = strtok (str," \r\t");
                while (pch != NULL) {
                    if (i == 0)
                         key = pch;
                    else
                        value = atof(pch);

                    pch = strtok (NULL, " \r\t");
                    i++;
                }
                if (key != "")
                    values.insert({key, value});


            }
            else {
                createMatrix(incr);
                int index_col = 0;
                float value = 0.0;

                pch = strtok (str," \n\r\t");
                while (pch != NULL) {
                    value = atof(pch);
                    matrix[index_row][index_col] = value;
                    pch = strtok (NULL, " \n\r\t");
                    index_col++;
                }


                index_row++;
            }

            contLine++;

            //free(str);
//            delete []str;
        }

        infile.close();
    }

    unordered_map<string,float> getValues() {
        return values;
    }

    float** getMatrix() {
        return matrix;
    }

    float getValueMatrix(int i, int j) {
        return matrix[i][j];
    }

    ~Reader() {

        for (int i = 0; i < values["nrows"]; i++)
            delete [] matrix[i];
        delete [] matrix;

        delete [] str;

    }
};


#endif

