#pragma once

struct ForwardBufferAddresses
{
	unsigned int vao = -1;

	// VBOs:
	unsigned int vertices = -1;
	unsigned int colors = -1;
	unsigned int normals = -1;
	unsigned int indices = -1;
	unsigned int textureCoordinates = -1;
};

// Buffer that contain the geometric information for the geometry pass
struct GeometryBuffersAddresses
{
	unsigned int gBuffer = -1;

	// Sub-buffers:
	unsigned int positions = -1;
	unsigned int albedosSpecular = -1;
	unsigned int normals = -1;
	unsigned int depths = -1;
};

// Buffer for an object with only one VBO
struct SingleBufferAddresses
{
	unsigned int vao = -1;
	unsigned int vbo = -1;
};
