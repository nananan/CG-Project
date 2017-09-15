#ifndef ALTITUDE_H
#define ALTITUDE_H

#include "../include/Include.h"

#include "string"
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <stdlib.h>

#define LINE 6

class Altitude
{
public:
    Altitude (int nrows, int ncols) {
        this->nrows = nrows+1;
        this->ncols = ncols+1;

        this->matrixVertex = new GLfloat*[this->nrows];
        for (int j = 0; j < this->nrows; j++)
            this->matrixVertex[j] = new GLfloat[this->ncols];
    }

    GLfloat getValueMatrix(int i, int j) {
        return matrixVertex[i][j];
    }

    void setValueMatrix(int i, int j, float z) {
        matrixVertex[i][j] += z;
    }


    void fillVertexMatrix(float** coordinates, int rows, int cols, int novalue) {
          for (int i = 0; i < rows+1; ++i) {
            for (int j = 0; j < cols+1; j++) {
                float h1, h2, h3, h4;
                if (i == 0 && j == 0) {
                        matrixVertex[i][j] = checkNoValue(coordinates, i ,j, novalue);
                }
                else if (i == 0) {
                    if (j <= cols-1) {
                        h1 = checkNoValue(coordinates, i ,j-1, novalue);
                        h2 = checkNoValue(coordinates, i ,j, novalue);
                        matrixVertex[i][j] = (h1+h2)/2;
                    }
                    else
                        matrixVertex[i][j] = checkNoValue(coordinates, i ,j-1, novalue);
                }
                else if (i == rows) {
                    if(j == 0){
                        matrixVertex[i][j] = checkNoValue(coordinates, i-1 ,j, novalue);
                    }
                    else if(j == cols) {
                        matrixVertex[i][j] = checkNoValue(coordinates, i-1 ,j-1, novalue);
                    }
                    else {
                        h1 = checkNoValue(coordinates, i-1 ,j-1, novalue);
                        h2 = checkNoValue(coordinates, i-1 ,j, novalue);
                        matrixVertex[i][j] = (h1+h2)/2;
                    }
                }
                else {
                    if (j == 0) {
                        h1 = checkNoValue(coordinates, i-1 ,j, novalue);
                        h2 = checkNoValue(coordinates, i ,j, novalue);
                        matrixVertex[i][j] = (h1+h2)/2;
                    }
                    else if (j == cols) {
                        h1 = checkNoValue(coordinates, i-1 ,j-1, novalue);
                        h2 = checkNoValue(coordinates, i ,j-1, novalue);
                        matrixVertex[i][j] = (h1+h2)/2;

                    }
                    else {
                        h1 = checkNoValue(coordinates, i ,j-1, novalue);
                        h2 = checkNoValue(coordinates, i ,j, novalue);
                        h3 = checkNoValue(coordinates, i-1 ,j, novalue);
                        h4 = checkNoValue(coordinates, i-1 ,j-1, novalue);
                        matrixVertex[i][j] = (h1+h2+h3+h4)/4;
                    }
                }
            }
        }
    }


    GLfloat** getMatrix() {
        return this->matrixVertex;
    }

    void calculateNormal() {
        float p1[3];
        float p2[3];
        float p3[3];

        int index = 1;
        float distance = 10.0;

        for (int i = 0; i < nrows; i++) {
            for (int j = 0; j < ncols; j++) {
                float valueX = i*distance;
                float valueY = j*distance;
                float valueZ = matrixVertex[i][j];

                if (index == 1) {
                    setPoint(p1, valueX, valueY, valueZ);
                }
                else if (index == 2) {
                    setPoint(p2, valueX, valueY, valueZ);
                }
                else if (index == 3) {
                    setPoint(p3, valueX, valueY, valueZ);

                    glm::vec3 normal = get_face_normal(p1,p2,p3);
                    vert_norm.push_back(make_tuple(normal.x, normal.y, normal.z));
                    index = 1;
                }
                index++;
            }
        }
    }

    tuple<float,float,float> getNormal(int index) {
        return make_tuple(get<0>(vert_norm[index]), get<1>(vert_norm[index]), get<2>(vert_norm[index]));
    }


    ~Altitude() {
        for (int i = 0; i < nrows; i++)
            delete [] matrixVertex[i];
        delete [] matrixVertex;

    }





private:
    GLfloat** matrixVertex;
    vector<tuple<float,float,float>> vert_norm;

    int nrows = 0;
    int ncols = 0;

    float checkNoValue(GLfloat** coordinates, int i, int j, int novalue){
        float height = 0;
        if (coordinates[i][j] != novalue) {
            height = coordinates[i][j];
        }
        return height;
    }

    // gets the normal of a face
    glm::vec3 get_face_normal(float p1[3], float p2[3], float p3[3])
    {
        glm::vec3 normal;

        float vect[2][3];
        float point[3][3];

        for (int i = 0; i < 3; ++i) { // copies points into point[][]
            point[0][i] = p1[i];
            point[1][i] = p2[i];
            point[2][i] = p3[i];
        }

        for (int i = 0; i < 2; ++i) { // calculates vectors from point[0] to point[1]
            for (int j = 0; j < 3; ++j) { // and point[0] to point[2]
                vect[i][j] = point[2-i][j] - point[0][j];
            }
        }

        glm::vec3 v = glm::vec3(vect[0][0], vect[0][1], vect[0][2]);
        glm::vec3 u = glm::vec3(vect[1][0], vect[1][1], vect[1][2]);

        // calculates vector at 90° to 2 vectors
        normal = glm::cross(v, u);

        // makes the vector length 1
        glm::normalize(normal);
        return normal;
    }


    void setPoint(float p[3], float x, float y, float z) {
        p[0] = x;
        p[1] = y;
        p[2] = z;
    }



};


#endif


