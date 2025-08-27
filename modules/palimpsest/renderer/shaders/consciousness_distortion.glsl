/*
 * Consciousness Distortion Vertex Shader
 * Warps geometry based on consciousness field effects
 * Used for buildings and environmental objects in Palimpsest
 */

shader_type spatial;
render_mode unshaded, cull_disabled, depth_draw_opaque, depth_test_disabled;

// Consciousness field parameters
uniform float consciousness_intensity : hint_range(-5.0, 10.0) = 0.0;
uniform int consciousness_type : hint_range(0, 7) = 0; // FieldType enum
uniform vec3 consciousness_center = vec3(0.0, 0.0, 0.0);
uniform float consciousness_radius : hint_range(0.1, 100.0) = 10.0;

// Distortion parameters
uniform float distortion_strength : hint_range(0.0, 5.0) = 1.0;
uniform float time_scale : hint_range(0.0, 10.0) = 1.0;
uniform bool enable_animation = true;

// Material parameters
uniform vec4 base_color : hint_color = vec4(0.7, 0.7, 0.7, 1.0);
uniform vec4 consciousness_tint : hint_color = vec4(1.0, 1.0, 1.0, 1.0);
uniform sampler2D main_texture : hint_default_white;
uniform float texture_scale : hint_range(0.1, 10.0) = 1.0;

// Bureaucratic stabilization
uniform bool bureaucratic_override = false;
uniform float form_stability : hint_range(0.0, 1.0) = 0.0;

varying vec3 world_pos;
varying vec3 original_pos;
varying float distortion_factor;
varying vec2 texture_uv;

// Consciousness field type effects
vec3 apply_depression_distortion(vec3 pos, float intensity, float time) {
    // Buildings droop downward, walls sag
    vec3 distorted = pos;
    
    // Vertical sagging based on height
    float sag_amount = intensity * 0.5 * smoothstep(0.0, 1.0, pos.y);
    distorted.y -= sag_amount;
    
    // Slight inward lean (depressive weight)
    vec2 center_pull = normalize(pos.xz) * intensity * 0.2;
    distorted.xz -= center_pull;
    
    // Gentle swaying motion
    if (enable_animation) {
        float sway = sin(time + pos.y * 2.0) * intensity * 0.1;
        distorted.x += sway;
    }
    
    return distorted;
}

vec3 apply_anxiety_distortion(vec3 pos, float intensity, float time) {
    // Chaotic, maze-like distortions
    vec3 distorted = pos;
    
    // Multi-frequency noise for chaos
    float noise1 = sin(pos.x * 5.0 + time) * cos(pos.z * 3.0 + time);
    float noise2 = cos(pos.y * 7.0 + time * 1.5) * sin(pos.x * 4.0);
    float noise3 = sin(pos.z * 6.0 + time * 0.7) * cos(pos.y * 8.0);
    
    vec3 chaotic_offset = vec3(noise1, noise2, noise3) * intensity * 0.15;
    distorted += chaotic_offset;
    
    // Rapid jittering
    if (enable_animation) {
        float jitter_freq = 20.0;
        vec3 jitter = vec3(
            sin(time * jitter_freq + pos.x),
            cos(time * jitter_freq * 1.3 + pos.y),
            sin(time * jitter_freq * 0.8 + pos.z)
        ) * intensity * 0.02;
        distorted += jitter;
    }
    
    return distorted;
}

vec3 apply_anger_distortion(vec3 pos, float intensity, float time) {
    // Sharp, aggressive, angular distortions
    vec3 distorted = pos;
    
    // Create sharp spikes and aggressive angles
    float spike_factor = step(0.7, sin(pos.x * 8.0) * cos(pos.z * 6.0));
    distorted.y += spike_factor * intensity * 0.8;
    
    // Sharp angular bends
    float bend_x = step(0.5, fract(pos.z * 4.0)) * 2.0 - 1.0;
    float bend_z = step(0.5, fract(pos.x * 3.0)) * 2.0 - 1.0;
    
    distorted.x += bend_x * intensity * 0.3;
    distorted.z += bend_z * intensity * 0.3;
    
    // Aggressive pulsing
    if (enable_animation) {
        float pulse = abs(sin(time * 5.0)) * intensity * 0.1;
        distorted *= (1.0 + pulse);
    }
    
    return distorted;
}

vec3 apply_memory_distortion(vec3 pos, float intensity, float time) {
    // Ghostly overlays, temporal echoes
    vec3 distorted = pos;
    
    // Create "echo" positions - multiple overlaid versions
    float echo1 = sin(time * 0.5) * 0.5 + 0.5;
    float echo2 = sin(time * 0.3 + 1.0) * 0.5 + 0.5;
    
    vec3 echo_offset1 = vec3(0.2, 0.1, -0.1) * intensity * echo1;
    vec3 echo_offset2 = vec3(-0.1, 0.3, 0.2) * intensity * echo2;
    
    // Blend between positions to create temporal uncertainty
    distorted = mix(distorted, distorted + echo_offset1, 0.3);
    distorted = mix(distorted, distorted + echo_offset2, 0.2);
    
    // Slow phase shifting
    if (enable_animation) {
        float phase = sin(time * 0.2) * intensity * 0.05;
        distorted += vec3(phase, -phase, phase * 0.5);
    }
    
    return distorted;
}

vec3 apply_bureaucratic_stabilization(vec3 pos, float stability) {
    // Force geometry to rigid, perpendicular angles
    vec3 stabilized = pos;
    
    if (stability > 0.0) {
        // Snap to grid-like positions
        float grid_size = mix(0.01, 0.5, stability);
        stabilized.x = round(stabilized.x / grid_size) * grid_size;
        stabilized.y = round(stabilized.y / grid_size) * grid_size;
        stabilized.z = round(stabilized.z / grid_size) * grid_size;
        
        // Reduce any distortions
        return mix(pos, stabilized, stability);
    }
    
    return pos;
}

vec3 apply_creative_distortion(vec3 pos, float intensity, float time) {
    // Impossible geometry, M.C. Escher effects
    vec3 distorted = pos;
    
    // Non-euclidean transformations
    float twist = pos.y * intensity * 0.5;
    mat3 twist_matrix = mat3(
        vec3(cos(twist), 0.0, sin(twist)),
        vec3(0.0, 1.0, 0.0),
        vec3(-sin(twist), 0.0, cos(twist))
    );
    
    distorted = twist_matrix * distorted;
    
    // Möbius strip-like effects
    float mobius = sin(pos.x + time) * intensity * 0.2;
    distorted.y += mobius * sin(pos.z * 2.0);
    
    return distorted;
}

vec3 apply_fear_distortion(vec3 pos, float intensity, float time) {
    // Stretching distances, closing walls, deepening shadows
    vec3 distorted = pos;
    
    // Walls lean inward menacingly
    vec2 inward_lean = -normalize(pos.xz) * intensity * 0.4;
    distorted.xz += inward_lean * smoothstep(0.0, 2.0, pos.y);
    
    // Distance stretching (make spaces feel larger/smaller)
    float distance_factor = 1.0 + intensity * 0.3;
    distorted.xz *= distance_factor;
    
    // Ominous swaying
    if (enable_animation) {
        float ominous_sway = sin(time * 0.8 + pos.y) * intensity * 0.08;
        distorted.x += ominous_sway;
    }
    
    return distorted;
}

vec3 apply_hope_distortion(vec3 pos, float intensity, float time) {
    // Opening spaces, gentle curves, upward lift
    vec3 distorted = pos;
    
    // Gentle upward lift
    distorted.y += intensity * 0.3 * smoothstep(0.0, 1.0, pos.y);
    
    // Outward expansion - spaces feel more open
    distorted.xz *= (1.0 + intensity * 0.2);
    
    // Gentle curves instead of sharp angles
    float curve_factor = sin(pos.x * 2.0) * cos(pos.z * 2.0) * intensity * 0.1;
    distorted.y += curve_factor;
    
    // Gentle swaying like growth
    if (enable_animation) {
        float growth_motion = sin(time * 0.3 + pos.y * 0.5) * intensity * 0.05;
        distorted += vec3(growth_motion * 0.5, growth_motion, growth_motion * 0.3);
    }
    
    return distorted;
}

void vertex() {
    world_pos = VERTEX;
    original_pos = VERTEX;
    
    // Calculate distance from consciousness center
    float distance_to_center = length(world_pos - consciousness_center);
    float field_influence = 1.0 - clamp(distance_to_center / consciousness_radius, 0.0, 1.0);
    
    // Apply field intensity falloff
    float effective_intensity = consciousness_intensity * field_influence * distortion_strength;
    distortion_factor = field_influence;
    
    vec3 distorted_vertex = world_pos;
    float time = TIME * time_scale;
    
    // Apply consciousness type-specific distortions
    if (consciousness_type == 0) { // FIELD_DEPRESSION
        distorted_vertex = apply_depression_distortion(world_pos, effective_intensity, time);
    } else if (consciousness_type == 1) { // FIELD_ANXIETY
        distorted_vertex = apply_anxiety_distortion(world_pos, effective_intensity, time);
    } else if (consciousness_type == 2) { // FIELD_ANGER
        distorted_vertex = apply_anger_distortion(world_pos, effective_intensity, time);
    } else if (consciousness_type == 3) { // FIELD_MEMORY
        distorted_vertex = apply_memory_distortion(world_pos, effective_intensity, time);
    } else if (consciousness_type == 4) { // FIELD_BUREAUCRATIC
        // Bureaucratic fields stabilize rather than distort
        distorted_vertex = apply_bureaucratic_stabilization(world_pos, abs(effective_intensity));
    } else if (consciousness_type == 5) { // FIELD_CREATIVE
        distorted_vertex = apply_creative_distortion(world_pos, effective_intensity, time);
    } else if (consciousness_type == 6) { // FIELD_FEAR
        distorted_vertex = apply_fear_distortion(world_pos, effective_intensity, time);
    } else if (consciousness_type == 7) { // FIELD_HOPE
        distorted_vertex = apply_hope_distortion(world_pos, effective_intensity, time);
    }
    
    // Apply bureaucratic override (forms can stabilize any distortion)
    if (bureaucratic_override) {
        distorted_vertex = apply_bureaucratic_stabilization(distorted_vertex, form_stability);
    }
    
    // Update vertex position
    VERTEX = distorted_vertex;
    world_pos = distorted_vertex;
    
    // Calculate texture coordinates
    texture_uv = UV * texture_scale;
}

void fragment() {
    // Sample main texture
    vec4 tex_color = texture(main_texture, texture_uv);
    vec4 final_color = base_color * tex_color;
    
    // Apply consciousness-based color tinting
    final_color = mix(final_color, consciousness_tint, distortion_factor * 0.5);
    
    // Special effects based on consciousness type
    if (consciousness_type == 0) { // Depression - desaturate
        float gray = dot(final_color.rgb, vec3(0.299, 0.587, 0.114));
        final_color.rgb = mix(final_color.rgb, vec3(gray), distortion_factor * 0.6);
    } else if (consciousness_type == 2) { // Anger - red tint
        final_color.r += distortion_factor * 0.3;
    } else if (consciousness_type == 3) { // Memory - ghostly transparency
        final_color.a *= (1.0 - distortion_factor * 0.4);
    } else if (consciousness_type == 4) { // Bureaucratic - rigid, clean
        // Quantize colors to create "official" look
        final_color.rgb = floor(final_color.rgb * 8.0) / 8.0;
    } else if (consciousness_type == 6) { // Fear - darken
        final_color.rgb *= (1.0 - distortion_factor * 0.4);
    } else if (consciousness_type == 7) { // Hope - brighten
        final_color.rgb += vec3(distortion_factor * 0.2);
    }
    
    ALBEDO = final_color.rgb;
    ALPHA = final_color.a;
}