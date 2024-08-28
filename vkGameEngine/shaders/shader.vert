#version 450

// input locations
layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;
// output locations
layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = vec4(position, 0.0, 1.0);
    fragColor = color;
}
