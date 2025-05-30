#pragma once

#include "common.hpp"

#define GL_VERSION_ES_CL_1_0              1
#define GL_VERSION_ES_CM_1_1              1
#define GL_VERSION_ES_CL_1_1              1
#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_STENCIL_BUFFER_BIT             0x00000400
#define GL_COLOR_BUFFER_BIT               0x00004000
#define GL_FALSE                          0
#define GL_TRUE                           1
#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_LOOP                      0x0002
#define GL_LINE_STRIP                     0x0003
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRIANGLE_FAN                   0x0006
#define GL_NEVER                          0x0200
#define GL_LESS                           0x0201
#define GL_EQUAL                          0x0202
#define GL_LEQUAL                         0x0203
#define GL_GREATER                        0x0204
#define GL_NOTEQUAL                       0x0205
#define GL_GEQUAL                         0x0206
#define GL_ALWAYS                         0x0207
#define GL_ZERO                           0
#define GL_ONE                            1
#define GL_SRC_COLOR                      0x0300
#define GL_ONE_MINUS_SRC_COLOR            0x0301
#define GL_SRC_ALPHA                      0x0302
#define GL_ONE_MINUS_SRC_ALPHA            0x0303
#define GL_DST_ALPHA                      0x0304
#define GL_ONE_MINUS_DST_ALPHA            0x0305
#define GL_DST_COLOR                      0x0306
#define GL_ONE_MINUS_DST_COLOR            0x0307
#define GL_SRC_ALPHA_SATURATE             0x0308
#define GL_CLIP_PLANE0                    0x3000
#define GL_CLIP_PLANE1                    0x3001
#define GL_CLIP_PLANE2                    0x3002
#define GL_CLIP_PLANE3                    0x3003
#define GL_CLIP_PLANE4                    0x3004
#define GL_CLIP_PLANE5                    0x3005
#define GL_FRONT                          0x0404
#define GL_BACK                           0x0405
#define GL_FRONT_AND_BACK                 0x0408
#define GL_FOG                            0x0B60
#define GL_LIGHTING                       0x0B50
#define GL_TEXTURE_2D                     0x0DE1
#define GL_CULL_FACE                      0x0B44
#define GL_ALPHA_TEST                     0x0BC0
#define GL_BLEND                          0x0BE2
#define GL_COLOR_LOGIC_OP                 0x0BF2
#define GL_DITHER                         0x0BD0
#define GL_STENCIL_TEST                   0x0B90
#define GL_DEPTH_TEST                     0x0B71
#define GL_POINT_SMOOTH                   0x0B10
#define GL_LINE_SMOOTH                    0x0B20
#define GL_SCISSOR_TEST                   0x0C11
#define GL_COLOR_MATERIAL                 0x0B57
#define GL_NORMALIZE                      0x0BA1
#define GL_RESCALE_NORMAL                 0x803A
#define GL_VERTEX_ARRAY                   0x8074
#define GL_NORMAL_ARRAY                   0x8075
#define GL_COLOR_ARRAY                    0x8076
#define GL_TEXTURE_COORD_ARRAY            0x8078
#define GL_MULTISAMPLE                    0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE       0x809E
#define GL_SAMPLE_ALPHA_TO_ONE            0x809F
#define GL_SAMPLE_COVERAGE                0x80A0
#define GL_NO_ERROR                       0
#define GL_INVALID_ENUM                   0x0500
#define GL_INVALID_VALUE                  0x0501
#define GL_INVALID_OPERATION              0x0502
#define GL_STACK_OVERFLOW                 0x0503
#define GL_STACK_UNDERFLOW                0x0504
#define GL_OUT_OF_MEMORY                  0x0505
#define GL_EXP                            0x0800
#define GL_EXP2                           0x0801
#define GL_FOG_DENSITY                    0x0B62
#define GL_FOG_START                      0x0B63
#define GL_FOG_END                        0x0B64
#define GL_FOG_MODE                       0x0B65
#define GL_FOG_COLOR                      0x0B66
#define GL_CW                             0x0900
#define GL_CCW                            0x0901
#define GL_CURRENT_COLOR                  0x0B00
#define GL_CURRENT_NORMAL                 0x0B02
#define GL_CURRENT_TEXTURE_COORDS         0x0B03
#define GL_POINT_SIZE                     0x0B11
#define GL_POINT_SIZE_MIN                 0x8126
#define GL_POINT_SIZE_MAX                 0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE      0x8128
#define GL_POINT_DISTANCE_ATTENUATION     0x8129
#define GL_SMOOTH_POINT_SIZE_RANGE        0x0B12
#define GL_LINE_WIDTH                     0x0B21
#define GL_SMOOTH_LINE_WIDTH_RANGE        0x0B22
#define GL_ALIASED_POINT_SIZE_RANGE       0x846D
#define GL_ALIASED_LINE_WIDTH_RANGE       0x846E
#define GL_CULL_FACE_MODE                 0x0B45
#define GL_FRONT_FACE                     0x0B46
#define GL_SHADE_MODEL                    0x0B54
#define GL_DEPTH_RANGE                    0x0B70
#define GL_DEPTH_WRITEMASK                0x0B72
#define GL_DEPTH_CLEAR_VALUE              0x0B73
#define GL_DEPTH_FUNC                     0x0B74
#define GL_STENCIL_CLEAR_VALUE            0x0B91
#define GL_STENCIL_FUNC                   0x0B92
#define GL_STENCIL_VALUE_MASK             0x0B93
#define GL_STENCIL_FAIL                   0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL        0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS        0x0B96
#define GL_STENCIL_REF                    0x0B97
#define GL_STENCIL_WRITEMASK              0x0B98
#define GL_MATRIX_MODE                    0x0BA0
#define GL_VIEWPORT                       0x0BA2
#define GL_MODELVIEW_STACK_DEPTH          0x0BA3
#define GL_PROJECTION_STACK_DEPTH         0x0BA4
#define GL_TEXTURE_STACK_DEPTH            0x0BA5
#define GL_MODELVIEW_MATRIX               0x0BA6
#define GL_PROJECTION_MATRIX              0x0BA7
#define GL_TEXTURE_MATRIX                 0x0BA8
#define GL_ALPHA_TEST_FUNC                0x0BC1
#define GL_ALPHA_TEST_REF                 0x0BC2
#define GL_BLEND_DST                      0x0BE0
#define GL_BLEND_SRC                      0x0BE1
#define GL_LOGIC_OP_MODE                  0x0BF0
#define GL_SCISSOR_BOX                    0x0C10
#define GL_COLOR_CLEAR_VALUE              0x0C22
#define GL_COLOR_WRITEMASK                0x0C23
#define GL_MAX_LIGHTS                     0x0D31
#define GL_MAX_CLIP_PLANES                0x0D32
#define GL_MAX_TEXTURE_SIZE               0x0D33
#define GL_MAX_MODELVIEW_STACK_DEPTH      0x0D36
#define GL_MAX_PROJECTION_STACK_DEPTH     0x0D38
#define GL_MAX_TEXTURE_STACK_DEPTH        0x0D39
#define GL_MAX_VIEWPORT_DIMS              0x0D3A
#define GL_MAX_TEXTURE_UNITS              0x84E2
#define GL_SUBPIXEL_BITS                  0x0D50
#define GL_RED_BITS                       0x0D52
#define GL_GREEN_BITS                     0x0D53
#define GL_BLUE_BITS                      0x0D54
#define GL_ALPHA_BITS                     0x0D55
#define GL_DEPTH_BITS                     0x0D56
#define GL_STENCIL_BITS                   0x0D57
#define GL_POLYGON_OFFSET_UNITS           0x2A00
#define GL_POLYGON_OFFSET_FILL            0x8037
#define GL_POLYGON_OFFSET_FACTOR          0x8038
#define GL_TEXTURE_BINDING_2D             0x8069
#define GL_VERTEX_ARRAY_SIZE              0x807A
#define GL_VERTEX_ARRAY_TYPE              0x807B
#define GL_VERTEX_ARRAY_STRIDE            0x807C
#define GL_NORMAL_ARRAY_TYPE              0x807E
#define GL_NORMAL_ARRAY_STRIDE            0x807F
#define GL_COLOR_ARRAY_SIZE               0x8081
#define GL_COLOR_ARRAY_TYPE               0x8082
#define GL_COLOR_ARRAY_STRIDE             0x8083
#define GL_TEXTURE_COORD_ARRAY_SIZE       0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE       0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE     0x808A
#define GL_VERTEX_ARRAY_POINTER           0x808E
#define GL_NORMAL_ARRAY_POINTER           0x808F
#define GL_COLOR_ARRAY_POINTER            0x8090
#define GL_TEXTURE_COORD_ARRAY_POINTER    0x8092
#define GL_SAMPLE_BUFFERS                 0x80A8
#define GL_SAMPLES                        0x80A9
#define GL_SAMPLE_COVERAGE_VALUE          0x80AA
#define GL_SAMPLE_COVERAGE_INVERT         0x80AB
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS     0x86A3
#define GL_DONT_CARE                      0x1100
#define GL_FASTEST                        0x1101
#define GL_NICEST                         0x1102
#define GL_PERSPECTIVE_CORRECTION_HINT    0x0C50
#define GL_POINT_SMOOTH_HINT              0x0C51
#define GL_LINE_SMOOTH_HINT               0x0C52
#define GL_FOG_HINT                       0x0C54
#define GL_GENERATE_MIPMAP_HINT           0x8192
#define GL_LIGHT_MODEL_AMBIENT            0x0B53
#define GL_LIGHT_MODEL_TWO_SIDE           0x0B52
#define GL_AMBIENT                        0x1200
#define GL_DIFFUSE                        0x1201
#define GL_SPECULAR                       0x1202
#define GL_POSITION                       0x1203
#define GL_SPOT_DIRECTION                 0x1204
#define GL_SPOT_EXPONENT                  0x1205
#define GL_SPOT_CUTOFF                    0x1206
#define GL_CONSTANT_ATTENUATION           0x1207
#define GL_LINEAR_ATTENUATION             0x1208
#define GL_QUADRATIC_ATTENUATION          0x1209
#define GL_BYTE                           0x1400
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_SHORT                          0x1402
#define GL_UNSIGNED_SHORT                 0x1403
#define GL_INT                            0x1404
#define GL_FLOAT                          0x1406
#define GL_FIXED                          0x140C
#define GL_CLEAR                          0x1500
#define GL_AND                            0x1501
#define GL_AND_REVERSE                    0x1502
#define GL_COPY                           0x1503
#define GL_AND_INVERTED                   0x1504
#define GL_NOOP                           0x1505
#define GL_XOR                            0x1506
#define GL_OR                             0x1507
#define GL_NOR                            0x1508
#define GL_EQUIV                          0x1509
#define GL_INVERT                         0x150A
#define GL_OR_REVERSE                     0x150B
#define GL_COPY_INVERTED                  0x150C
#define GL_OR_INVERTED                    0x150D
#define GL_NAND                           0x150E
#define GL_SET                            0x150F
#define GL_EMISSION                       0x1600
#define GL_SHININESS                      0x1601
#define GL_AMBIENT_AND_DIFFUSE            0x1602
#define GL_MODELVIEW                      0x1700
#define GL_PROJECTION                     0x1701
#define GL_TEXTURE                        0x1702
#define GL_ALPHA                          0x1906
#define GL_RGB                            0x1907
#define GL_RGBA                           0x1908
#define GL_LUMINANCE                      0x1909
#define GL_LUMINANCE_ALPHA                0x190A
#define GL_UNPACK_ALIGNMENT               0x0CF5
#define GL_PACK_ALIGNMENT                 0x0D05
#define GL_UNSIGNED_SHORT_4_4_4_4         0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1         0x8034
#define GL_UNSIGNED_SHORT_5_6_5           0x8363
#define GL_FLAT                           0x1D00
#define GL_SMOOTH                         0x1D01
#define GL_KEEP                           0x1E00
#define GL_REPLACE                        0x1E01
#define GL_INCR                           0x1E02
#define GL_DECR                           0x1E03
#define GL_VENDOR                         0x1F00
#define GL_RENDERER                       0x1F01
#define GL_VERSION                        0x1F02
#define GL_EXTENSIONS                     0x1F03
#define GL_MODULATE                       0x2100
#define GL_DECAL                          0x2101
#define GL_ADD                            0x0104
#define GL_TEXTURE_ENV_MODE               0x2200
#define GL_TEXTURE_ENV_COLOR              0x2201
#define GL_TEXTURE_ENV                    0x2300
#define GL_NEAREST                        0x2600
#define GL_LINEAR                         0x2601
#define GL_NEAREST_MIPMAP_NEAREST         0x2700
#define GL_LINEAR_MIPMAP_NEAREST          0x2701
#define GL_NEAREST_MIPMAP_LINEAR          0x2702
#define GL_LINEAR_MIPMAP_LINEAR           0x2703
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
#define GL_GENERATE_MIPMAP                0x8191
#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE1                       0x84C1
#define GL_TEXTURE2                       0x84C2
#define GL_TEXTURE3                       0x84C3
#define GL_TEXTURE4                       0x84C4
#define GL_TEXTURE5                       0x84C5
#define GL_TEXTURE6                       0x84C6
#define GL_TEXTURE7                       0x84C7
#define GL_TEXTURE8                       0x84C8
#define GL_TEXTURE9                       0x84C9
#define GL_TEXTURE10                      0x84CA
#define GL_TEXTURE11                      0x84CB
#define GL_TEXTURE12                      0x84CC
#define GL_TEXTURE13                      0x84CD
#define GL_TEXTURE14                      0x84CE
#define GL_TEXTURE15                      0x84CF
#define GL_TEXTURE16                      0x84D0
#define GL_TEXTURE17                      0x84D1
#define GL_TEXTURE18                      0x84D2
#define GL_TEXTURE19                      0x84D3
#define GL_TEXTURE20                      0x84D4
#define GL_TEXTURE21                      0x84D5
#define GL_TEXTURE22                      0x84D6
#define GL_TEXTURE23                      0x84D7
#define GL_TEXTURE24                      0x84D8
#define GL_TEXTURE25                      0x84D9
#define GL_TEXTURE26                      0x84DA
#define GL_TEXTURE27                      0x84DB
#define GL_TEXTURE28                      0x84DC
#define GL_TEXTURE29                      0x84DD
#define GL_TEXTURE30                      0x84DE
#define GL_TEXTURE31                      0x84DF
#define GL_ACTIVE_TEXTURE                 0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE          0x84E1
#define GL_REPEAT                         0x2901
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_LIGHT0                         0x4000
#define GL_LIGHT1                         0x4001
#define GL_LIGHT2                         0x4002
#define GL_LIGHT3                         0x4003
#define GL_LIGHT4                         0x4004
#define GL_LIGHT5                         0x4005
#define GL_LIGHT6                         0x4006
#define GL_LIGHT7                         0x4007
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_ARRAY_BUFFER_BINDING           0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING   0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING    0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING    0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING     0x8898
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING 0x889A
#define GL_STATIC_DRAW                    0x88E4
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_BUFFER_SIZE                    0x8764
#define GL_BUFFER_USAGE                   0x8765
#define GL_SUBTRACT                       0x84E7
#define GL_COMBINE                        0x8570
#define GL_COMBINE_RGB                    0x8571
#define GL_COMBINE_ALPHA                  0x8572
#define GL_RGB_SCALE                      0x8573
#define GL_ADD_SIGNED                     0x8574
#define GL_INTERPOLATE                    0x8575
#define GL_CONSTANT                       0x8576
#define GL_PRIMARY_COLOR                  0x8577
#define GL_PREVIOUS                       0x8578
#define GL_OPERAND0_RGB                   0x8590
#define GL_OPERAND1_RGB                   0x8591
#define GL_OPERAND2_RGB                   0x8592
#define GL_OPERAND0_ALPHA                 0x8598
#define GL_OPERAND1_ALPHA                 0x8599
#define GL_OPERAND2_ALPHA                 0x859A
#define GL_ALPHA_SCALE                    0x0D1C
#define GL_SRC0_RGB                       0x8580
#define GL_SRC1_RGB                       0x8581
#define GL_SRC2_RGB                       0x8582
#define GL_SRC0_ALPHA                     0x8588
#define GL_SRC1_ALPHA                     0x8589
#define GL_SRC2_ALPHA                     0x858A
#define GL_DOT3_RGB                       0x86AE
#define GL_DOT3_RGBA                      0x86AF
#define GL_UNIFORM_BUFFER                 0x8A11
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_UNSIGNED_INT                   0x1405
#define GL_LINE                           0x1B01
#define GL_FILL                           0x1B02
#define GL_RED                            0x1903

using GLbyte     = i8;
using GLshort    = i16;
using GLint      = i32;
using GLint64    = i64;

using GLubyte    = u8;
using GLushort   = u16;
using GLuint     = u32;
using GLuint64   = u64;

using GLfloat    = f32;
using GLdouble   = f64;
using GLclampf   = f32;
using GLclampd   = f64;

using GLsizeiptr = iptr;
using GLintptr   = iptr;
using Glbitfield = u32;
using GLsizei    = i32;
using GLenum     = u32;

using GLchar     = char;
using GLvoid     = void;
using GLboolean  = u8;

using Type_glViewport                 = void (GLint x, GLint y, GLsizei width, GLsizei height);
using Type_glClearColor               = void (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
using Type_glClear                    = void (Glbitfield mask);
using Type_glGetString                = const GLubyte *(GLenum name);
using Type_glGenBuffers               = void (GLsizei n, GLuint *buffers);
using Type_glGenVertexArrays          = void (GLsizei n, GLuint *arrays);
using Type_glGenTextures              = void (GLsizei n, GLuint *textures);
using Type_glBindBuffer               = void (GLenum target, GLuint buffer);
using Type_glBindVertexArray          = void (GLuint array);
using Type_glBindTexture              = void (GLenum target, GLuint texture);
using Type_glBufferData               = void (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
using Type_glCreateShader             = GLuint (GLenum shaderType);
using Type_glShaderSource             = void (GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
using Type_glCompileShader            = void (GLuint shader);
using Type_glGetShaderiv              = void (GLuint shader, GLenum pname, GLint *params);
using Type_glGetProgramiv             = void (GLuint program, GLenum pname, GLint *params);
using Type_glGetShaderInfoLog         = void (GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
using Type_glGetProgramInfoLog        = void (GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
using Type_glCreateProgram            = GLuint (void);
using Type_glAttachShader             = void (GLuint program, GLuint shader);
using Type_glLinkProgram              = void (GLuint program);
using Type_glUseProgram               = void (GLuint program);
using Type_glDeleteShader             = void (GLuint shader);
using Type_glVertexAttribPointer      = void (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
using Type_glEnableVertexAttribArray  = void (GLuint index);
using Type_glDisableVertexAttribArray = void (GLuint index);
using Type_glDrawArrays               = void (GLenum mode, GLint first, GLsizei count);
using Type_glDrawElements             = void (GLenum mode, GLsizei count, GLenum type, const GLvoid *indicies);
using Type_glPolygonMode              = void (GLenum face, GLenum mode);
using Type_glGetUniformLocation       = GLint (GLuint program, const GLchar *name);
using Type_glTexParameterfv           = void(GLenum target, GLenum pname, const GLfloat *params);
using Type_glTexParameteriv           = void(GLenum target, GLenum pname, const GLint *params);
using Type_glTexParameterIiv          = void(GLenum target, GLenum pname, const GLint *params);
using Type_glTexParameterIuiv         = void(GLenum target, GLenum pname, const GLuint *params);
using Type_glTexParameterf            = void (GLenum target, GLenum pname, GLfloat param);
using Type_glTexParameteri            = void (GLenum target, GLenum pname, GLint param);
using Type_glTexImage2D               = void (GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data);
using Type_glGenerateMipmap           = void (GLenum target);
using Type_glEnable                   = void (GLenum cap);
using Type_glDisable                  = void (GLenum cap);
using Type_glEnablei                  = void (GLenum cap, GLuint index);
using Type_glDisablei                 = void (GLenum cap, GLuint index);
using Type_glBlendFunc                = void (GLenum sfactor, GLenum dfactor);
using Type_glDepthFunc                = void (GLenum func);
using Type_glBlendFunci               = void (GLuint buf, GLenum sfactor, GLenum dfactor);
using Type_glBindVertexBuffer         = void (GLuint bindingindex, GLuint buffer, GLintptr offset, GLintptr stride);
using Type_glVertexAttribBinding      = void (GLuint attribindex, GLuint bindingindex);
using Type_glVertexAttribFormat       = void (GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
using Type_glVertexAttribIFormat      = void (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
using Type_glDepthRange               = void (GLdouble nearVal, GLdouble farVal);
using Type_glDepthRangef              = void (GLfloat nearVal, GLfloat farVal);
using Type_glBindBufferBase           = void (GLenum target, GLuint index, GLuint buffer);


using Type_glUniform1f          = void (GLint location, GLfloat v0);
using Type_glUniform2f          = void (GLint location, GLfloat v0, GLfloat v1);
using Type_glUniform3f          = void (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
using Type_glUniform4f          = void (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
using Type_glUniform1i          = void (GLint location, GLint v0);
using Type_glUniform2i          = void (GLint location, GLint v0, GLint v1);
using Type_glUniform3i          = void (GLint location, GLint v0, GLint v1, GLint v2);
using Type_glUniform4i          = void (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
using Type_glUniform1ui         = void (GLint location, GLuint v0);
using Type_glUniform2ui         = void (GLint location, GLuint v0, GLuint v1);
using Type_glUniform3ui         = void (GLint location, GLuint v0, GLuint v1, GLuint v2);
using Type_glUniform4ui         = void (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
using Type_glUniform1fv         = void (GLint location, GLsizei count, const GLfloat *value);
using Type_glUniform2fv         = void (GLint location, GLsizei count, const GLfloat *value);
using Type_glUniform3fv         = void (GLint location, GLsizei count, const GLfloat *value);
using Type_glUniform4fv         = void (GLint location, GLsizei count, const GLfloat *value);
using Type_glUniform1iv         = void (GLint location, GLsizei count, const GLint *value);
using Type_glUniform2iv         = void (GLint location, GLsizei count, const GLint *value);
using Type_glUniform3iv         = void (GLint location, GLsizei count, const GLint *value);
using Type_glUniform4iv         = void (GLint location, GLsizei count, const GLint *value);
using Type_glUniform1uiv        = void (GLint location, GLsizei count, const GLuint *value);
using Type_glUniform2uiv        = void (GLint location, GLsizei count, const GLuint *value);
using Type_glUniform3uiv        = void (GLint location, GLsizei count, const GLuint *value);
using Type_glUniform4uiv        = void (GLint location, GLsizei count, const GLuint *value);
using Type_glUniformMatrix2fv   = void (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
using Type_glUniformMatrix3fv   = void (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
using Type_glUniformMatrix4fv   = void (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
using Type_glUniformMatrix2x3fv = void (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
using Type_glUniformMatrix3x2fv = void (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
using Type_glUniformMatrix2x4fv = void (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
using Type_glUniformMatrix4x2fv = void (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
using Type_glUniformMatrix3x4fv = void (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
using Type_glUniformMatrix4x3fv = void (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
using Type_glGetError           = GLenum (void);
using Type_glFinish             = void (void);

#define OPENGL_FUNC_PTR(FUNC_NAME) Type_##FUNC_NAME *FUNC_NAME
struct OpenGL {
    OPENGL_FUNC_PTR(glViewport);
    OPENGL_FUNC_PTR(glClearColor);
    OPENGL_FUNC_PTR(glClear);
    OPENGL_FUNC_PTR(glGetString);
    OPENGL_FUNC_PTR(glGenBuffers);
    OPENGL_FUNC_PTR(glGenVertexArrays);
    OPENGL_FUNC_PTR(glGenTextures);
    OPENGL_FUNC_PTR(glBindBuffer);
    OPENGL_FUNC_PTR(glBindTexture);
    OPENGL_FUNC_PTR(glBufferData);
    OPENGL_FUNC_PTR(glCreateShader);
    OPENGL_FUNC_PTR(glShaderSource);
    OPENGL_FUNC_PTR(glCompileShader);
    OPENGL_FUNC_PTR(glGetShaderiv);
    OPENGL_FUNC_PTR(glGetProgramiv);
    OPENGL_FUNC_PTR(glGetShaderInfoLog);
    OPENGL_FUNC_PTR(glGetProgramInfoLog);
    OPENGL_FUNC_PTR(glCreateProgram);
    OPENGL_FUNC_PTR(glAttachShader);
    OPENGL_FUNC_PTR(glLinkProgram);
    OPENGL_FUNC_PTR(glUseProgram);
    OPENGL_FUNC_PTR(glDeleteShader);
    OPENGL_FUNC_PTR(glVertexAttribPointer);
    OPENGL_FUNC_PTR(glEnableVertexAttribArray);
    OPENGL_FUNC_PTR(glDisableVertexAttribArray);
    OPENGL_FUNC_PTR(glBindVertexArray);
    OPENGL_FUNC_PTR(glDrawArrays);
    OPENGL_FUNC_PTR(glDrawElements);
    OPENGL_FUNC_PTR(glPolygonMode);
    OPENGL_FUNC_PTR(glGetUniformLocation);
    OPENGL_FUNC_PTR(glTexParameterfv);
    OPENGL_FUNC_PTR(glTexParameteriv);
    OPENGL_FUNC_PTR(glTexParameterIiv);
    OPENGL_FUNC_PTR(glTexParameterIuiv);
    OPENGL_FUNC_PTR(glTexParameterf);
    OPENGL_FUNC_PTR(glTexParameteri);
    OPENGL_FUNC_PTR(glTexImage2D);
    OPENGL_FUNC_PTR(glGenerateMipmap);
    OPENGL_FUNC_PTR(glEnable);
    OPENGL_FUNC_PTR(glDisable);
    OPENGL_FUNC_PTR(glEnablei);
    OPENGL_FUNC_PTR(glDisablei);
    OPENGL_FUNC_PTR(glBlendFunc);
    OPENGL_FUNC_PTR(glBlendFunci);
    OPENGL_FUNC_PTR(glDepthFunc);
    OPENGL_FUNC_PTR(glUniform1f);
    OPENGL_FUNC_PTR(glUniform2f);
    OPENGL_FUNC_PTR(glUniform3f);
    OPENGL_FUNC_PTR(glUniform4f);
    OPENGL_FUNC_PTR(glUniform1i);
    OPENGL_FUNC_PTR(glUniform2i);
    OPENGL_FUNC_PTR(glUniform3i);
    OPENGL_FUNC_PTR(glUniform4i);
    OPENGL_FUNC_PTR(glUniform1ui);
    OPENGL_FUNC_PTR(glUniform2ui);
    OPENGL_FUNC_PTR(glUniform3ui);
    OPENGL_FUNC_PTR(glUniform4ui);
    OPENGL_FUNC_PTR(glUniform1fv);
    OPENGL_FUNC_PTR(glUniform2fv);
    OPENGL_FUNC_PTR(glUniform3fv);
    OPENGL_FUNC_PTR(glUniform4fv);
    OPENGL_FUNC_PTR(glUniform1iv);
    OPENGL_FUNC_PTR(glUniform2iv);
    OPENGL_FUNC_PTR(glUniform3iv);
    OPENGL_FUNC_PTR(glUniform4iv);
    OPENGL_FUNC_PTR(glUniform1uiv);
    OPENGL_FUNC_PTR(glUniform2uiv);
    OPENGL_FUNC_PTR(glUniform3uiv);
    OPENGL_FUNC_PTR(glUniform4uiv);
    OPENGL_FUNC_PTR(glUniformMatrix2fv);
    OPENGL_FUNC_PTR(glUniformMatrix3fv);
    OPENGL_FUNC_PTR(glUniformMatrix4fv);
    OPENGL_FUNC_PTR(glUniformMatrix2x3fv);
    OPENGL_FUNC_PTR(glUniformMatrix3x2fv);
    OPENGL_FUNC_PTR(glUniformMatrix2x4fv);
    OPENGL_FUNC_PTR(glUniformMatrix4x2fv);
    OPENGL_FUNC_PTR(glUniformMatrix3x4fv);
    OPENGL_FUNC_PTR(glUniformMatrix4x3fv);
    OPENGL_FUNC_PTR(glGetError);
    OPENGL_FUNC_PTR(glFinish);
    OPENGL_FUNC_PTR(glBindVertexBuffer);
    OPENGL_FUNC_PTR(glVertexAttribBinding);
    OPENGL_FUNC_PTR(glVertexAttribFormat);
    OPENGL_FUNC_PTR(glVertexAttribIFormat);
    OPENGL_FUNC_PTR(glDepthRange);
    OPENGL_FUNC_PTR(glDepthRangef);
    OPENGL_FUNC_PTR(glBindBufferBase);
};
#undef OPENGL_FUNC_PTR
