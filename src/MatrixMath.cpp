#include "MatrixMath.h"


using Matrix = std::vector<std::vector<float>>;

//matrix multiplication with othe matrix or vector
Matrix MatrixMath::matrixMatrixMultiply(Matrix A, Matrix B)
{
    float rowsA = A.size();
    float colsA = A[0].size();
    float colsB = B[0].size();

    // Check if the matrices can be multiplied
    if (colsA != B.size()) {
        return Matrix();
    }

    Matrix result(4, std::vector<float>(4, 0));

    //multiply
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return result;
}
Vector3 MatrixMath::matrixVectorMultiply(Matrix matrix, Vector3 v) {
    float result[4] = {0,0,0,0};
    float vector[4];
    vector[0] = v.x;
    vector[1] = v.y;
    vector[2] = v.z;
    vector[3] = 1;


    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
    Vector3 r (result[0], result[1], result[2]);
    return r;
}

//special case for the porjection matrix, so it can be "normalized" later
std::vector<float>  MatrixMath::projectMatrixMultiply(Matrix matrix, Vector3 v) {
    std::vector<float> result = { 0,0,0,0};
    float vector[4];
    vector[0] = v.x;
    vector[1] = v.y;
    vector[2] = v.z;
    vector[3] = 1;

    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
    return result;
}

//translation
Vector3 MatrixMath::translate(Vector3 translationVector, Vector3 point)
{
    Matrix t = createTranslationMatrix(translationVector);
    Vector3 newPoint = matrixVectorMultiply(t, point);
    return newPoint;
}

Matrix MatrixMath::createTranslationMatrix(Vector3 v) {
    Matrix translationMatrix(4, std::vector<float>(4, 0.0f));
    translationMatrix[0][0] = 1.0f;
    translationMatrix[1][1] = 1.0f;
    translationMatrix[2][2] = 1.0f;
    translationMatrix[3][3] = 1.0f;
    translationMatrix[0][3] = v.x;
    translationMatrix[1][3] = v.y;
    translationMatrix[2][3] = v.z;
    return translationMatrix;
}

//rotation
Vector3 MatrixMath::rotate(Vector3 rotationVector, Vector3 point)
{
    Matrix t = createRotationMatrixFromEulerAngles(rotationVector);
    Vector3 newPoint = matrixVectorMultiply(t, point);
    return newPoint;
}

Matrix MatrixMath::createRotationMatrixFromEulerAngles(const Vector3& euler) {
    float cosX = std::cos(( M_PI/ 180)*euler.x);
    float sinX = std::sin((M_PI / 180) * euler.x);
    float cosY = std::cos((M_PI / 180) * euler.y);
    float sinY = std::sin((M_PI / 180) * euler.y);
    float cosZ = std::cos((M_PI / 180) * euler.z);
    float sinZ = std::sin((M_PI / 180) * euler.z);

    Matrix rotationX(4, std::vector<float>(4, 0.0f));
    rotationX[0][0] = 1.0f;
    rotationX[1][1] = cosX;
    rotationX[1][2] = -sinX;
    rotationX[2][1] = sinX;
    rotationX[2][2] = cosX;
    rotationX[3][3] = 1.0f;

    Matrix rotationY(4, std::vector<float>(4, 0.0f));
    rotationY[0][0] = cosY;
    rotationY[0][2] = sinY;
    rotationY[1][1] = 1.0f;
    rotationY[2][0] = -sinY;
    rotationY[2][2] = cosY;
    rotationY[3][3] = 1.0f;

    Matrix rotationZ(4, std::vector<float>(4, 0.0f));
    rotationZ[0][0] = cosZ;
    rotationZ[0][1] = -sinZ;
    rotationZ[1][0] = sinZ;
    rotationZ[1][1] = cosZ;
    rotationZ[2][2] = 1.0f;
    rotationZ[3][3] = 1.0f;

    // Combine the individual rotation matrices (order may vary depending on the desired rotation order)
    // In this example, we use the order ZYX (roll, pitch, yaw)
    
    Matrix rotationMatrix = matrixMatrixMultiply(rotationZ, rotationY);
    rotationMatrix = matrixMatrixMultiply(rotationMatrix, rotationX);

    return rotationMatrix;
}

//scaling
Vector3 MatrixMath::scale(Vector3 scaleVector, Vector3 point)
{
    Matrix t = createScaleMatrix(scaleVector);
    Vector3 newPoint = matrixVectorMultiply(t, point);
    return newPoint;
}

Matrix MatrixMath::createScaleMatrix(Vector3 scaleVector) {
    Matrix scaleMatrix(4, std::vector<float>(4, 0.0f));
    scaleMatrix[0][0] = scaleVector.x;
    scaleMatrix[1][1] = scaleVector.y;
    scaleMatrix[2][2] = scaleVector.z;
    scaleMatrix[3][3] = 1.0f; // The fourth row is usually set to (0, 0, 0, 1) for affine transformations

    return scaleMatrix;
}