#version 150 core
uniform mat4 modelView;
in vec4 position;

void main()
{
    gl_Position = modelView * position;
}
