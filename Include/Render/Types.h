#ifndef _RE_TYPES_H_
#define _RE_TYPES_H_

#include <Engine/Types.h>

// Resource types
#define RES_MODEL		"Model"
#define RES_TEXTURE		"Texture"
#define RES_MATERIAL	"Material"

enum IndexType
{
	IT_UINT_8,
	IT_UINT_16,
	IT_UINT_32
};

enum PrimitiveTopology
{
	PT_TRIANGLES,
	PT_POINTS,
	PT_LINES
};

enum ClearType
{
	CT_COLOR	= 0x00000001,
	CT_DEPTH	= 0x00000002,
	CT_STENCIL	= 0x00000004
};

enum BufferType
{
	BT_VERTEX_BUFFER,
	BT_INDEX_BUFFER,
	BT_UNIFORM_BUFFER
};

enum TextureType
{
	TT_2D,
	TT_CUBE
};

enum TextureFormat
{
	TF_R8G8B8,
	TF_R8G8B8A8,
	TF_R5G6G5,
	TF_A8,
	TF_L8,
	TF_BC1,
	TF_BC2,
	TF_BC3,
	TF_BC4,
	TF_BC5,
	TF_D16,
	TF_D32,
	TF_D24S8
};

enum CullMode
{
	CM_NONE,
	CM_BACK,
	CM_FRONT,
	CM_FRONT_AND_BACK
};

enum ShaderStage
{
	SS_VERTEX,
	SS_FRAGMENT
};

enum BufferUsage
{
	BU_STATIC,
	BU_DYNAMIC
};

enum BlendFactor
{
	BF_ZERO,
	BF_ONE,
	BF_SRC_COLOR,
	BF_ONE_MINUS_SRC_COLOR,
	BF_DST_COLOR,
	BF_ONE_MINUS_DST_COLOR,
	BF_SRC_ALPHA,
	BF_ONE_MINUS_SRC_ALPHA,
	BF_DST_ALPHA,
	BF_ONE_MINUS_DST_ALPHA,
	BF_CONSTANT_COLOR,
	BF_ONE_MINUS_CONSTANT_COLOR,
	BF_CONSTANT_ALPHA,
	BF_ONE_MINUS_CONSTANT_ALPHA,
	BF_SRC_ALPHA_SATURATE,
	BF_SRC1_COLOR,
	BF_ONE_MINUS_SRC1_COLOR,
	BF_SRC1_ALPHA,
	BF_ONE_MINUS_SRC1_ALPHA
};

struct Model;
struct ModelCreateInfo;

struct RenderDriver;
struct RenderDevice;
struct RenderDeviceInfo;
struct RenderDeviceProcs;
struct RenderContextProcs;

struct Shader;
struct ShaderModule;

struct Texture;
struct TextureCreateInfo;

struct Buffer;
struct BufferCreateInfo;

struct InputLayout;
struct InputLayoutDesc;
struct BindingLayoutDesc;

struct Drawable;

#endif /* _RE_TYPES_H_ */
