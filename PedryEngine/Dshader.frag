#version 430 core

in float instanceID;

out vec4 FragColor;

void main()
{

    //float variation = (instanceID / 8000.0) / 4.0;
    FragColor = vec4(0.25, 0.25, 0.25 / 0.25, 0.1);

}