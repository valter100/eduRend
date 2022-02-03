#pragma once
#include "Model.h"

class Cube : public Model
{
	unsigned nbr_indices = 0;

public:
	mat4f transform;
	mat4f rotation;
	mat4f scale;

	mat4f transformMatrix;

	Cube* parent;

	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;
	Vertex   v101, v102, v103, v104
			,v201, v202, v203, v204
			,v301, v302, v303, v304
			,v401, v402, v403, v404
			,v501, v502, v503, v504
			,v601, v602, v603, v604;


	//Material baseMaterial;

	Cube(ID3D11Device*, ID3D11DeviceContext*, Cube*);
	~Cube() {};
	void Render() const; //Override from model?
	void AssignParent(Cube& cube);
	Cube* GetParent();
	void GiveVertexValues(Vertex vertex, vec3f pos, vec3f normal, vec2f texCoord);
	void CreateTriangle(int index1, int index2, int index3);

	void SetTransform(mat4f _transform);
	mat4f GetTransform();

	void SetRotation(mat4f _rotation);
	mat4f GetRotation();

	void SetScale(mat4f _scale);
	mat4f GetScale();

	mat4f GetParentTransformAndRotation();
	mat4f InheritTransformAndRotation();

	void SetTransformMatrix(mat4f transform, mat4f rotation, mat4f scale);
	void SetTransformMatrix(mat4f _transformMatrix);
	mat4f GetTransformMatrix();

	void LoadTexture();
}
;