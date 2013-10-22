uniform sampler2D u_diffuse_texture;
varying vec2 v_uvcoord0;

const float blurSize = 1.0/100.0;
const float weightSum = (70.0 + 2.0 * (1.0 + 8.0 + 28.0 + 56.0));

void main()
{
    vec4 sum = vec4(0.0, 0.0, 0.0, 1.0);
    
    sum += texture2D(u_diffuse_texture, vec2(v_uvcoord0.x - 4.0*blurSize, v_uvcoord0.y)) * 1.0 / weightSum;
    sum += texture2D(u_diffuse_texture, vec2(v_uvcoord0.x - 3.0*blurSize, v_uvcoord0.y)) * 8.0 / weightSum;
    sum += texture2D(u_diffuse_texture, vec2(v_uvcoord0.x - 2.0*blurSize, v_uvcoord0.y)) * 28.0 / weightSum;
    sum += texture2D(u_diffuse_texture, vec2(v_uvcoord0.x - blurSize, v_uvcoord0.y)) * 56.0 / weightSum;
    sum += texture2D(u_diffuse_texture, vec2(v_uvcoord0.x, v_uvcoord0.y)) * 70.0 / weightSum;
    sum += texture2D(u_diffuse_texture, vec2(v_uvcoord0.x + blurSize, v_uvcoord0.y)) * 56.0 / weightSum;
    sum += texture2D(u_diffuse_texture, vec2(v_uvcoord0.x + 2.0*blurSize, v_uvcoord0.y)) * 28.0 / weightSum;
    sum += texture2D(u_diffuse_texture, vec2(v_uvcoord0.x + 3.0*blurSize, v_uvcoord0.y)) * 8.0 / weightSum;
    sum += texture2D(u_diffuse_texture, vec2(v_uvcoord0.x + 4.0*blurSize, v_uvcoord0.y)) * 1.0 / weightSum;
    
    gl_FragColor = sum;
}
