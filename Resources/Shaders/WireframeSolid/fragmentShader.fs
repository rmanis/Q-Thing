#version 400

in vec3 fColor;
in vec3 bCoord;
layout(location = 0, index = 0) out vec4 fragmentColor;

void main() {
    float DISCARD_THRESHOLD = 0.1;
    float LINE_THRESHOLD = 0.02;
    vec3 FILL_COLOR = vec3(0.05, 0.06, 0.07);
    //fragmentColor = vec4(0.5, 0.5, 1.0, 1.0);
    float mindist = min(bCoord.x, min(bCoord.y, bCoord.z));
    // if (mindist > DISCARD_THRESHOLD) discard;
    vec3 color = mindist > LINE_THRESHOLD ? FILL_COLOR : fColor;
    fragmentColor = vec4(color, 1.0);
}

