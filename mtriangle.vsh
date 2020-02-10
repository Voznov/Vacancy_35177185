attribute highp vec4 posAttr;
attribute lowp vec4 colLeftAttr;
attribute lowp vec4 colRightAttr;
attribute highp float timeAttr;
varying lowp vec4 col;
uniform highp mat4 matrix;
highp float diffTimeAttr;

void main() {
    diffTimeAttr = abs(timeAttr);
    col = colLeftAttr * diffTimeAttr + colRightAttr * (1.0f - diffTimeAttr);
    gl_Position = matrix * posAttr;
}
