#version 400

in vec3 fragColor;
layout(location = 0, index = 0) out vec4 fragmentColor;

void main() {
    //fragmentColor = vec4(0.5, 0.5, 1.0, 1.0);
    fragmentColor = vec4(fragColor, 1.0);
}

