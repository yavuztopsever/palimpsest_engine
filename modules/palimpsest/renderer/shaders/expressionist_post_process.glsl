/*
 * Expressionist Post-Process Shader
 * Creates German Expressionist visual effects for Palimpsest Engine
 * - High contrast chiaroscuro lighting
 * - Monochrome palette with red accents
 * - Consciousness-driven distortion
 */

shader_type canvas_item;

// Consciousness parameters
uniform float consciousness_distortion : hint_range(0.0, 10.0) = 0.0;
uniform float depression_intensity : hint_range(0.0, 1.0) = 0.0;
uniform float anxiety_intensity : hint_range(0.0, 1.0) = 0.0;
uniform float anger_intensity : hint_range(0.0, 1.0) = 0.0;

// Visual style parameters
uniform float contrast : hint_range(0.5, 3.0) = 1.8;
uniform float brightness : hint_range(0.0, 2.0) = 1.0;
uniform float desaturation : hint_range(0.0, 1.0) = 0.9;
uniform vec3 red_accent_color : hint_color = vec3(0.8, 0.1, 0.1);
uniform float red_accent_threshold : hint_range(0.0, 1.0) = 0.3;

// Distortion parameters  
uniform float distortion_strength : hint_range(0.0, 0.5) = 0.1;
uniform float time_scale : hint_range(0.0, 5.0) = 1.0;

// Brush stroke simulation
uniform float brush_stroke_intensity : hint_range(0.0, 1.0) = 0.3;
uniform sampler2D brush_noise_texture : hint_default_black;

// Fog and atmosphere
uniform float fog_density : hint_range(0.0, 1.0) = 0.2;
uniform vec3 fog_color : hint_color = vec3(0.6, 0.6, 0.6);

varying vec2 world_position;

vec3 rgb_to_hsv(vec3 c) {
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
    
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv_to_rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

// Consciousness-driven distortion
vec2 apply_consciousness_distortion(vec2 uv, float time) {
    vec2 distorted_uv = uv;
    
    // Depression: downward pull and blur
    if (depression_intensity > 0.0) {
        float depression_offset = sin(uv.x * 10.0 + time) * depression_intensity * 0.02;
        distorted_uv.y += depression_offset;
        distorted_uv.y -= depression_intensity * 0.05; // Overall downward pull
    }
    
    // Anxiety: chaotic distortion
    if (anxiety_intensity > 0.0) {
        float noise_x = sin(uv.y * 20.0 + time * 2.0) * cos(uv.x * 15.0);
        float noise_y = cos(uv.x * 18.0 + time * 1.5) * sin(uv.y * 12.0);
        distorted_uv += vec2(noise_x, noise_y) * anxiety_intensity * 0.01;
    }
    
    // Anger: sharp, angular distortions
    if (anger_intensity > 0.0) {
        float sharp_x = step(0.5, fract(uv.x * 8.0)) * 2.0 - 1.0;
        float sharp_y = step(0.5, fract(uv.y * 6.0)) * 2.0 - 1.0;
        distorted_uv += vec2(sharp_x, sharp_y) * anger_intensity * 0.008;
    }
    
    // General consciousness distortion
    if (consciousness_distortion > 0.0) {
        vec2 center = vec2(0.5, 0.5);
        vec2 offset = uv - center;
        float dist = length(offset);
        float distortion_amount = consciousness_distortion * 0.1 * sin(dist * 10.0 + time);
        distorted_uv += normalize(offset) * distortion_amount * 0.01;
    }
    
    return distorted_uv;
}

// Convert to monochrome with selective red preservation
vec3 apply_expressionist_palette(vec3 color) {
    // Convert to HSV to analyze color
    vec3 hsv = rgb_to_hsv(color);
    
    // Check if color is in red range (preserve some red tones)
    bool is_red_accent = (hsv.x > 0.9 || hsv.x < 0.1) && hsv.y > red_accent_threshold;
    
    if (is_red_accent && anger_intensity > 0.3) {
        // Preserve and enhance red accents during anger
        return mix(vec3(dot(color, vec3(0.299, 0.587, 0.114))), red_accent_color, 0.7);
    } else {
        // Convert to monochrome
        float luminance = dot(color, vec3(0.299, 0.587, 0.114));
        vec3 monochrome = vec3(luminance);
        
        // Apply slight desaturation rather than pure grayscale
        return mix(color, monochrome, desaturation);
    }
}

// High contrast chiaroscuro effect
vec3 apply_chiaroscuro(vec3 color) {
    // Increase contrast dramatically
    vec3 contrasted = pow(color, vec3(1.0 / contrast));
    
    // Crush blacks and boost whites for dramatic lighting
    contrasted = smoothstep(0.1, 0.9, contrasted);
    
    // Apply brightness
    contrasted *= brightness;
    
    return contrasted;
}

// Simulate brush stroke texture
vec3 apply_brush_strokes(vec3 color, vec2 uv) {
    if (brush_stroke_intensity <= 0.0) return color;
    
    // Sample brush noise texture
    vec3 brush_noise = texture(brush_noise_texture, uv * 4.0).rgb;
    
    // Create brush stroke pattern
    float stroke_pattern = sin(uv.x * 100.0) * cos(uv.y * 80.0);
    stroke_pattern = smoothstep(0.2, 0.8, stroke_pattern + 0.5);
    
    // Combine noise and pattern
    float brush_mask = mix(1.0, brush_noise.r * stroke_pattern, brush_stroke_intensity);
    
    return color * brush_mask;
}

// Atmospheric fog effect
vec3 apply_atmospheric_fog(vec3 color, vec2 uv) {
    if (fog_density <= 0.0) return color;
    
    // Create depth-based fog (assume UV.y represents depth)
    float depth_factor = smoothstep(0.2, 1.0, uv.y);
    
    // Add consciousness-influenced fog density
    float consciousness_fog = depression_intensity * 0.5 + anxiety_intensity * 0.3;
    float total_fog = clamp(fog_density + consciousness_fog, 0.0, 0.9);
    
    // Apply fog mixing
    return mix(color, fog_color, total_fog * depth_factor);
}

void fragment() {
    float time = TIME * time_scale;
    
    // Apply consciousness distortion to UV
    vec2 distorted_uv = apply_consciousness_distortion(UV, time);
    
    // Clamp UV to prevent sampling outside texture
    distorted_uv = clamp(distorted_uv, 0.0, 1.0);
    
    // Sample the original texture
    vec3 original_color = texture(TEXTURE, distorted_uv).rgb;
    
    // Apply expressionist color palette
    vec3 color = apply_expressionist_palette(original_color);
    
    // Apply high contrast chiaroscuro
    color = apply_chiaroscuro(color);
    
    // Apply brush stroke simulation
    color = apply_brush_strokes(color, UV);
    
    // Apply atmospheric fog
    color = apply_atmospheric_fog(color, UV);
    
    // Handle reality breakdown at extreme consciousness levels
    if (consciousness_distortion > 8.0) {
        // Reality is breaking down - add static/noise
        float noise = fract(sin(dot(UV * time, vec2(12.9898, 78.233))) * 43758.5453);
        color = mix(color, vec3(noise), 0.3);
    }
    
    COLOR = vec4(color, 1.0);
}