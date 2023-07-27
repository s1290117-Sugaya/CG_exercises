varying vec4 oPosition;
varying vec4 oNormal;

vec3 colHash(int i, int j){
    float r = float(i);
    float g = float(j);
    float b = float(i+j);
    
    for(int i=0; i<2; i++){
	r = mod(87.0*r + 23.0*g + 125.0*b, 257.0);
	g = mod(87.0*r + 23.0*g + 125.0*b, 1009.0);
	b = mod(87.0*r + 23.0*g + 125.0*b, 21001.0);
    }

    return vec3(r/257.0, g/1009.0, b/21001.0);
}

vec2 dispHash(int i, int j){
    float x = float(i);
    float y = float(j);

    for(int i=0; i<2; i++){
	x = mod(87.0*x + 23.0*y, 257.0);
	y = mod(87.0*x + 23.0*y, 1009.0);
    }

    return vec2(x/257.0, y/1009.0);
}

vec3 vor2d(vec2 pos){
    float step = 10.0;
    int xi = int(floor(pos.x/step));
    int yj = int(floor(pos.y/step));
    ivec2 nearest;
    float min_dist = 1e5;

    for(int i=xi-1; i<=xi+1; i++){
	for(int j=yj-1; j<=yj+1; j++){
	    vec2 disp = dispHash(i, j);
	    vec2 seed = vec2((float(i)+disp.x)*step, (float(j)+disp.y)*step);
	    float dist = length(pos-seed);
	    if(dist<min_dist){
		min_dist = dist;
		nearest = ivec2(i, j);
	    }
	}
    }

    vec3 col = colHash(nearest.x, nearest.y);
    return col;
}

void main(){
    vec2 p = gl_FragCoord.xy;
    vec3 col = vor2d(p);

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
