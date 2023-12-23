#pragma once
#include <glm/glm.hpp>
#include <ngui/types.hxx>

class ColorScheme
{
    public:
        enum Catpuccin : u32 {
            Rosewater 	= 0xf5e0dc,
            Flamingo  	= 0xf2cdcd,
            Pink      	= 0xf5c2e7,
            Mauve     	= 0xcba6f7,
            Red       	= 0xf38ba8,
            Maroon    	= 0xeba0ac,
            Peach     	= 0xfab387,
            Yellow    	= 0xf9e2af,
            Green     	= 0xa6e3a1,
            Teal      	= 0x94e2d5,
            Sky       	= 0x89dceb,
            Sapphire  	= 0x74c7ec,
            Blue      	= 0x89b4fa,
            Lavender  	= 0xb4befe,
            Text      	= 0xcdd6f4,
            Subtext1  	= 0xbac2de,
            Subtext0  	= 0xa6adc8,
            Overlay2  	= 0x9399b2,
            Overlay1  	= 0x7f849c,
            Overlay0  	= 0x6c7086,
            Surface2  	= 0x585b70,
            Surface1  	= 0x45475a,
            Surface0  	= 0x313244,
            Base      	= 0x1e1e2e,
            Mantle    	= 0x181825,
            Crust     	= 0x11111b
        };

        ColorScheme() = default;
        constexpr ColorScheme(Catpuccin color) : value(color) { }

        // Allow switch and comparisons.
        constexpr operator Catpuccin() const { return value; }

        // Prevent usage: if(fruit)
        explicit operator bool() const = delete;        
        constexpr bool operator==(ColorScheme a) const { return value == a.value; }
        constexpr bool operator!=(ColorScheme a) const { return value != a.value; }

        constexpr auto vec3_normalized() -> glm::vec3 {
            f32 b = static_cast<f32>(static_cast<u32>(value) & 0x0000FFu) / 255.0f;
            f32 g = static_cast<f32>((static_cast<u32>(value) & 0x00FF00u) >> 8u) / 255.0f;
            f32 r = static_cast<f32>((static_cast<u32>(value) & 0xFF0000u) >> 16u) / 255.0f;
            return glm::vec3(r, g, b);
        }

    private:
        Catpuccin value;
};

