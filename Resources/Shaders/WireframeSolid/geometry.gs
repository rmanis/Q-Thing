#version 400
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 vColor[];
out vec3 fColor;
out vec3 bCoord;

void main(void) {
    gl_Position = gl_in[0].gl_Position;
    fColor = vColor[0];
    bCoord = vec3(1.0, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    fColor = vColor[1];
    bCoord = vec3(0.0, 1.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    fColor = vColor[2];
    bCoord = vec3(0.0, 0.0, 1.0);
    EmitVertex();

    EndPrimitive();
}
