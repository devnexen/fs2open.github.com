#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "graphics/grinternal.h"
#include "model/model.h"

enum class ComparisionFunction
{
	Never,
	Always,
	Less,
	Greater,
	Equal,
	NotEqual,
	LessOrEqual,
	GreaterOrEqual
};

enum class StencilOperation {
	Keep,
	Zero,
	Replace,
	Increment,
	IncrementWrap,
	Decrement,
	DecrementWrap,
	Invert
};

class material
{
public:
	struct fog 
	{
		bool enabled;
		int r;
		int g;
		int b;
		float dist_near;
		float dist_far;
	};

	struct clip_plane
	{
		bool enabled;
		vec3d normal;
		vec3d position;
	};

	enum texture_type {
		TEX_TYPE_NORMAL,
		TEX_TYPE_XPARENT,
		TEX_TYPE_AABITMAP,
		TEX_TYPE_INTERFACE
	};

	struct StencilFunc {
		ComparisionFunction compare = ComparisionFunction::Always;
		int ref = 1;
		uint32_t mask = 0xFFFFFFFF;
	};

	struct StencilOp {
		StencilOperation stencilFailOperation = StencilOperation::Keep;
		StencilOperation depthFailOperation = StencilOperation::Keep;
		StencilOperation successOperation = StencilOperation::Keep;
	};

private:
	shader_type Sdr_type;

	int Texture_maps[TM_NUM_TYPES];
	texture_type Tex_type;

	clip_plane Clip_params;
	int Texture_addressing;
	fog Fog_params;
	gr_zbuffer_type Depth_mode;
	gr_alpha_blend Blend_mode;
	bool Cull_mode;
	int Fill_mode;
	vec4 Clr;
	float Clr_scale;
	int Depth_bias;
	bvec4 Color_mask;
	bool Stencil_test = false;
	uint32_t Stencil_mask = 0xFF;
	StencilFunc Stencil_func;
	StencilOp Front_stencil_op;
	StencilOp Back_stencil_op;

protected:
	void set_shader_type(shader_type init_sdr_type = SDR_TYPE_NONE);
public:
	material();

	int get_shader_handle() const;
	virtual uint get_shader_flags() const;

	void set_texture_map(int tex_type, int texture_num);
	int get_texture_map(int tex_type) const;
	bool is_textured() const;

	void set_texture_type(texture_type t_type);
	int get_texture_type() const;
	
	bool is_clipped() const;
	void set_clip_plane(const vec3d &normal, const vec3d &position);
	void set_clip_plane();
	const clip_plane& get_clip_plane() const;

	void set_texture_addressing(int addressing);
	int get_texture_addressing() const;

	void set_fog(int r, int g, int b, float near, float far);
	void set_fog();
	bool is_fogged() const;
	const fog& get_fog() const;

	void set_depth_mode(gr_zbuffer_type mode);
	gr_zbuffer_type get_depth_mode() const;

	void set_cull_mode(bool mode);
	bool get_cull_mode() const;

	void set_fill_mode(int mode);
	int get_fill_mode() const;

	void set_color_mask(bool red, bool green, bool blue, bool alpha);
	const bvec4& get_color_mask() const;

	void set_blend_mode(gr_alpha_blend mode);
	gr_alpha_blend get_blend_mode() const;

	void set_depth_bias(int bias);
	int get_depth_bias() const;

	void set_color(float red, float green, float blue, float alpha);
	void set_color(int r, int g, int b, int a);
	void set_color(color &clr_in);
	const vec4& get_color() const;

	void set_color_scale(float scale);
	float get_color_scale() const;

	void set_stencil_test(bool stencil);
	bool is_stencil_enabled() const;

	void set_stencil_mask(uint32_t mask);
	uint32_t get_stencil_mask() const;

	void set_stencil_func(ComparisionFunction compare, int ref, uint32_t mask);
	const StencilFunc& get_stencil_func() const;

	void set_front_stencil_op(StencilOperation stencilFailOperation,
							  StencilOperation depthFailOperation,
							  StencilOperation successOperation);
	const StencilOp& get_front_stencil_op() const;

	void set_back_stencil_op(StencilOperation stencilFailOperation,
							  StencilOperation depthFailOperation,
							  StencilOperation successOperation);
	const StencilOp& get_back_stencil_op() const;
};

class model_material : public material
{
	bool Desaturate = false;

	bool Shadow_casting = false;
	bool Batched = false;

	bool Deferred = false;
	bool HDR = false;
	bool lighting = false;
	float Light_factor = 1.0f;

	int Center_alpha = 0;

	int Animated_effect = -1;
	float Animated_timer = 0.0f;

	float Thrust_scale = -1.0f;

	bool Team_color_set = false;
	team_color Tm_color;

	bool Normal_alpha = false;
	float Normal_alpha_min = 0.0f;
	float Normal_alpha_max = 1.0f;

	bool Normal_extrude = false;
	float Normal_extrude_width = -1.0f;

public:
	model_material();

	void set_desaturation(bool enabled);
	bool is_desaturated() const;

	void set_shadow_casting(bool enabled);
	bool is_shadow_casting() const;

	void set_light_factor(float factor);
	float get_light_factor() const;

	void set_lighting(bool mode);
	bool is_lit() const;

	void set_deferred_lighting(bool enabled);
	void set_high_dynamic_range(bool enabled);
	
	void set_center_alpha(int center_alpha);
	int get_center_alpha() const;

	void set_thrust_scale(float scale = -1.0f);
	float get_thrust_scale() const;

	void set_team_color(const team_color &Team_clr);
	void set_team_color();
	const team_color& get_team_color() const;

	void set_animated_effect(int effect, float time);
	void set_animated_effect();
	int get_animated_effect() const;
	float get_animated_effect_time() const;

	void set_normal_alpha(float min, float max);
	void set_normal_alpha();
	bool is_normal_alpha_active() const;
	float get_normal_alpha_min() const;
	float get_normal_alpha_max() const;

	void set_normal_extrude(float width);
	void set_normal_extrude();
	bool is_normal_extrude_active() const;
	float get_normal_extrude_width() const;

	void set_batching(bool enabled);
	bool is_batched() const;

	virtual uint get_shader_flags() const override;
};

class particle_material : public material
{
	bool Point_sprite;
public:
	particle_material();

	void set_point_sprite_mode(bool enabled);
	bool get_point_sprite_mode();

	uint get_shader_flags() const override;
};

class distortion_material: public material
{
	bool Thruster_render;
public:
	distortion_material();

	void set_thruster_rendering(bool enabled);
	bool get_thruster_rendering();
};

class shield_material : public material
{
	matrix Impact_orient;
	vec3d Impact_pos;
	float Impact_radius;
public:
	shield_material();

	void set_impact_transform(matrix &orient, vec3d &pos);
	void set_impact_radius(float radius);

	const matrix& get_impact_orient();
	const vec3d& get_impact_pos();
	float get_impact_radius();
};

class movie_material : public material {
	int Ytex = -1;
	int Utex = -1;
	int Vtex = -1;
 public:
	movie_material();

	int getYtex() const;
	int getUtex() const;
	int getVtex() const;

	void setYtex(int _Ytex);
	void setUtex(int _Utex);
	void setVtex(int _Vtex);
};

class batched_bitmap_material : public material
{
 public:
	batched_bitmap_material();
};

class nanovg_material : public material {
 public:
	nanovg_material();
};

gr_alpha_blend material_determine_blend_mode(int base_bitmap, bool is_transparent);
gr_zbuffer_type material_determine_depth_mode(bool depth_testing, bool is_transparent);

void material_set_interface(material* mat_info, int texture, bool blended, float alpha);
void material_set_unlit(material* mat_info, int texture, float alpha, bool blending, bool depth_testing);
void material_set_unlit_emissive(material* mat_info, int texture, float alpha, float color_scale);
void material_set_unlit_color(material* mat_info, int texture, color *clr, bool blending, bool depth_testing);
void material_set_unlit_color(material* mat_info, int texture, color *clr, float alpha, bool blending, bool depth_testing);
void material_set_unlit_volume(particle_material* mat_info, int texture, bool point_sprites);
void material_set_distortion(distortion_material *mat_info, int texture, bool thruster);
void material_set_movie(movie_material *mat_info, int y_bm, int u_bm, int v_bm);
void material_set_batched_bitmap(batched_bitmap_material* mat_info, int base_tex, float alpha, float color_scale);
void material_set_nanovg(nanovg_material* mat_info, int base_tex);

#endif
