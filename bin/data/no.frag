#version 150

uniform sampler2DRect tex0;
uniform int frameNumber;
uniform vec2 resolution;
uniform float timetime;
in vec2 texCoordVarying;
out vec4 outputColor;

//http://webstaff.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf
//simplex pretty much 99% copied from there
//adjusted by getting "completely random" gradients instead of randoming from 12 preset ones
//and normalizing the gradient vector

float noise3D(vec3 p)
{
    return fract(sin(dot(p ,vec3(12.9898,78.233,128.852))) * 43758.5453)*2.0-1.0;
}

float simplex3D(vec3 p)
{
    
    float f3 = 1.0/3.0;
    float s = (p.x+p.y+p.z)*f3;
    int i = int(floor(p.x+s));
    int j = int(floor(p.y+s));
    int k = int(floor(p.z+s));
    
    float g3 = 1.0/6.0;
    float t = float((i+j+k))*g3;
    float x0 = float(i)-t;
    float y0 = float(j)-t;
    float z0 = float(k)-t;
    x0 = p.x-x0;
    y0 = p.y-y0;
    z0 = p.z-z0;
    
    int i1,j1,k1;
    int i2,j2,k2;
    
    if(x0>=y0)
    {
        if(y0>=z0){ i1=1; j1=0; k1=0; i2=1; j2=1; k2=0; } // X Y Z order
        else if(x0>=z0){ i1=1; j1=0; k1=0; i2=1; j2=0; k2=1; } // X Z Y order
        else { i1=0; j1=0; k1=1; i2=1; j2=0; k2=1; }  // Z X Z order
    }
    else 
    { 
        if(y0<z0) { i1=0; j1=0; k1=1; i2=0; j2=1; k2=1; } // Z Y X order
        else if(x0<z0) { i1=0; j1=1; k1=0; i2=0; j2=1; k2=1; } // Y Z X order
        else { i1=0; j1=1; k1=0; i2=1; j2=1; k2=0; } // Y X Z order
    }
    
    float x1 = x0 - float(i1) + g3; 
    float y1 = y0 - float(j1) + g3;
    float z1 = z0 - float(k1) + g3;
    float x2 = x0 - float(i2) + 2.0*g3; 
    float y2 = y0 - float(j2) + 2.0*g3;
    float z2 = z0 - float(k2) + 2.0*g3;
    float x3 = x0 - 1.0 + 3.0*g3; 
    float y3 = y0 - 1.0 + 3.0*g3;
    float z3 = z0 - 1.0 + 3.0*g3;   
                 
    vec3 ijk0 = vec3(i,j,k);
    vec3 ijk1 = vec3(i+i1,j+j1,k+k1);   
    vec3 ijk2 = vec3(i+i2,j+j2,k+k2);
    vec3 ijk3 = vec3(i+1,j+1,k+1);  
            
    vec3 gr0 = normalize(vec3(noise3D(ijk0),noise3D(ijk0*2.01),noise3D(ijk0*2.02)));
    vec3 gr1 = normalize(vec3(noise3D(ijk1),noise3D(ijk1*2.01),noise3D(ijk1*2.02)));
    vec3 gr2 = normalize(vec3(noise3D(ijk2),noise3D(ijk2*2.01),noise3D(ijk2*2.02)));
    vec3 gr3 = normalize(vec3(noise3D(ijk3),noise3D(ijk3*2.01),noise3D(ijk3*2.02)));
    
    float n0 = 0.0;
    float n1 = 0.0;
    float n2 = 0.0;
    float n3 = 0.0;

    float t0 = 0.5 - x0*x0 - y0*y0 - z0*z0;
    if(t0>=0.0)
    {
        t0*=t0;
        n0 = t0 * t0 * dot(gr0, vec3(x0, y0, z0));
    }
    float t1 = 0.5 - x1*x1 - y1*y1 - z1*z1;
    if(t1>=0.0)
    {
        t1*=t1;
        n1 = t1 * t1 * dot(gr1, vec3(x1, y1, z1));
    }
    float t2 = 0.5 - x2*x2 - y2*y2 - z2*z2;
    if(t2>=0.0)
    {
        t2 *= t2;
        n2 = t2 * t2 * dot(gr2, vec3(x2, y2, z2));
    }
    float t3 = 0.5 - x3*x3 - y3*y3 - z3*z3;
    if(t3>=0.0)
    {
        t3 *= t3;
        n3 = t3 * t3 * dot(gr3, vec3(x3, y3, z3));
    }
    return 96.0*(n0+n1+n2+n3);
    
}

float fbm(vec3 p)
{
    float f;
    f  = 0.50000*simplex3D( p ); p = p*2.01;
    f += 0.25000*simplex3D( p ); p = p*2.02; //from iq
    f += 0.12500*simplex3D( p ); p = p*2.03;
    f += 0.06250*simplex3D( p ); p = p*2.04;
    f += 0.03125*simplex3D( p );
    return f;
}

vec3 offsetNoise(vec3 pos) {
    return vec3(
        simplex3D(pos + vec3(-120.34, +340.21, -13.67)),
        simplex3D(pos + vec3(+12.301, +70.261, -167.678)),
        simplex3D(pos + vec3(+78.676, -178.678, -79.612))
    );
}

vec3 turbulence(vec3 pos, float mystery) {
    int octaves = 8;
    vec3 lucanarity = vec3(2.0);
    vec3 gain = vec3(0.5) * mystery;
    vec3 scale = vec3(0.5);

    vec3 v = vec3(0.0);

    for (int i = 0; i < octaves; i++) {
        vec3 octave = vec3(float(i * 2));

        vec3 newPos = pos * pow(lucanarity, octave) * mystery;
        v += scale * pow(gain, octave) * abs(offsetNoise(newPos));
    }

    return v;
}

//https://www.shadertoy.com/view/XljGzV
vec3 rgb2hsl(vec3 c){
  float h = 0.0;
    float s = 0.0;
    float l = 0.0;
    float r = c.r;
    float g = c.g;
    float b = c.b;
    float cMin = min( r, min( g, b ) );
    float cMax = max( r, max( g, b ) );

    l = ( cMax + cMin ) / 2.0;
    if ( cMax > cMin ) {
        float cDelta = cMax - cMin;
        
        //s = l < .05 ? cDelta / ( cMax + cMin ) : cDelta / ( 2.0 - ( cMax + cMin ) ); Original
        s = l < .0 ? cDelta / ( cMax + cMin ) : cDelta / ( 2.0 - ( cMax + cMin ) );
        
        if ( r == cMax ) {
            h = ( g - b ) / cDelta;
        } else if ( g == cMax ) {
            h = 2.0 + ( b - r ) / cDelta;
        } else {
            h = 4.0 + ( r - g ) / cDelta;
        }

        if ( h < 0.0) {
            h += 6.0;
        }
        h = h / 6.0;
    }
    return vec3( h, s, l );
}
vec3 hsl2rgb(vec3 c)
{
    vec3 rgb = clamp( abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );

    return c.z + c.y * (rgb-0.5)*(1.0-abs(2.0*c.z-1.0));
}
// vec3 hueShift (vec3 color, float shift)
// {
//     vec3 P = vec3(0.55735)*dot(vec3(0.55735),color);
    
//     vec3 U = color-P;
    
//     vec3 V = cross(vec3(0.55735),U);    

//     color = U*cos(shift*6.2832) + V*sin(shift*6.2832) + P;
    
//     return vec3(color);
// }
vec3 naiveHueShift(vec3 color, float shift) {
    vec3 hsl = rgb2hsl(color);
    hsl.x = hsl.x + shift;
    return hsl2rgb(hsl);
}

vec3 adjustHsl(vec3 rgb, vec3 shift) {
    vec3 hsl = rgb2hsl(rgb);
    hsl = hsl + shift;
    return hsl2rgb(hsl);
}

void main() {
    vec4 drawMask = texture(tex0, texCoordVarying);
    float time = ((float(frameNumber) / 10.0) + 1.) / 25.;
    vec2 uv = (gl_FragCoord.xy/resolution.xy - 0.5) * vec2(resolution.x / resolution.y, 1);
    // vec2 uv = gl_FragCoord.xy / resolution.xy / 2;
    vec3 pos = vec3(uv, time);
    vec3 turb = turbulence(pos, drawMask.r);
    vec3 c = naiveHueShift(vec3(-1.4654, 2.9033, 0), turb.r);
    c = adjustHsl(vec3(uv, time), c);
    float g = hsl2rgb(c).g;
    outputColor = vec4(vec3(1, 0, 0.3), normalize(sqrt(g)));
}
