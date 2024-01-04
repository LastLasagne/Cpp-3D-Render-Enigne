#pragma once


#ifndef MatrixMath_H
#define MatrixMath_H
#define _USE_MATH_DEFINES

#include <vector>
#include "Vector3.h"
#include <math.h>
#include <cmath>
#endif

class MatrixMath
{
	using Matrix = std::vector<std::vector<float>>;

public:
    //different matrix operations used in render pipeline
    Matrix matrixMatrixMultiply(Matrix A, Matrix B);

    Vector3 matrixVectorMultiply(Matrix matrix, Vector3 vector);
	
    Vector3 translate(Vector3 translationVector, Vector3 point);

    Matrix createTranslationMatrix(Vector3 v);

    Vector3 rotate(Vector3 rotationVector, Vector3 point);

    Matrix createRotationMatrixFromEulerAngles(const Vector3& euler);

    Vector3 scale(Vector3 scaleVector, Vector3 point);

    Matrix createScaleMatrix(Vector3 scaleVector);

    std::vector<float> projectMatrixMultiply(Matrix matrix, Vector3 v);


};

