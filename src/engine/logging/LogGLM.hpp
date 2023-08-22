#pragma once

#include "Log.hpp"
#include "glm/glm.hpp"

inline void PrintMat4(const glm::mat4& mat) {
	LEI_INFO("ROW 0: {0}, {1}, {2}, {3}", mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
	LEI_INFO("ROW 1: {0}, {1}, {2}, {3}", mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
	LEI_INFO("ROW 2: {0}, {1}, {2}, {3}", mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
	LEI_INFO("ROW 3: {0}, {1}, {2}, {3}", mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
	// mat[2][0], mat[2][1], mat[2][2], mat[2][3],
	// mat[3][0], mat[3][1], mat[3][2], mat[3][3],
}

inline void PrintVec3(std::string& name, const glm::vec3& vec) {
	LEI_INFO("{0}: ({1}, {2}, {3})", name, vec[0], vec[1], vec[2]);
}