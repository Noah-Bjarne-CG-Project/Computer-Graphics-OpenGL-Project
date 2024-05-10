#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform float time;
uniform vec2 resolution;

float hash(float n) { return fract(sin(n) * 43758.5453123); }
float noise(vec2 x) {
    vec2 p = floor(x);
    vec2 f = fract(x);
    f = f * f * (3.0 - 2.0 * f);
    float n = p.x + p.y * 57.0;
    return mix(mix(hash(n + 0.0), hash(n + 1.0), f.x),
               mix(hash(n + 57.0), hash(n + 58.0), f.x), f.y);
}

void main() {
    vec2 uv = gl_FragCoord.xy / resolution;
    float t = time * 0.05;
    uv.y += t;

    float n = noise(uv * 10.0);
    vec3 rainColor = vec3(0.5, 0.5, 0.8) * n; 
    float brightness = smoothstep(0.9, 1.0, n); 

    FragColor = vec4(rainColor * brightness, 1.0);
}
