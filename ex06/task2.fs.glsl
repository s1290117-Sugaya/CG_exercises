varying vec4 oPosition;
varying vec4 oNormal;

vec3 bri2d(vec2 pos){
    vec3 brickColor = vec3(0.70, 0.32, 0.20);
    vec3 mortarColor = vec3(0.50, 0.25, 0.10);
    vec2 brickSize = vec2(0.08, 0.04);
    vec2 brickPct = vec2(0.9, 0.8975);
    vec2 position = pos / brickSize;

    if(fract(position.y*0.5) > 0.5) position.x += 0.5;
    position = fract(position);
    vec2 useBrick = step(position, brickPct);
    vec3 col = mix(mortarColor, brickColor, useBrick.x*useBrick.y);

    return col;
}

vec2 map(vec3 p){
    return vec2(acos(p.x/length(p)), atan(p.y/p.z));
}

void main(){
    vec2 p = map(oPosition.xyz);
    vec3 col = bri2d(p);

    vec4 light = vec4(10.0, 5.0, 1.0, 1.0);
    vec4 V = oPosition;
    vec4 L = normalize(light - V);
    vec4 N = oNormal;

    vec4 amb = vec4(0.1, 0.1, 0.1, 1.0);
    vec4 diff = vec4(col, 1.0);
    vec4 spec = vec4(1.0, 1.0, 1.0, 1.0);
    float shiny = 16.0;

    vec4 lcol = vec4(1.0, 1.0, 1.0, 1.0);

    vec4 R = reflect(-L, N);
    vec4 View = normalize(-V);

    float ndl = clamp(dot(N, L), 0.0, 1.0);
    float vdr = pow(clamp(dot(View, R), 0.0, 1.0), shiny);

    vec4 lin = vec4(0.0);
    lin += amb * lcol;
    lin += ndl * diff * lcol;
    lin += vdr * spec * lcol;

    gl_FragColor = lin;
}
