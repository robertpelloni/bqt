package theme

import (
	"image/color"
)

type ThemeType int

const (
	Dark ThemeType = iota
	Cyberpunk
	LiquidGlass
	Aetheria // The Antigravity Original
)

type Theme struct {
	Type       ThemeType
	Primary    color.NRGBA
	Surface    color.NRGBA
	Background color.NRGBA
	Text       color.NRGBA
	Accent     color.NRGBA
}

func GetTheme(t ThemeType) Theme {
	switch t {
	case Cyberpunk:
		return Theme{
			Type:       Cyberpunk,
			Primary:    color.NRGBA{R: 0, G: 240, B: 255, A: 255}, // Neon Cyan
			Surface:    color.NRGBA{R: 10, G: 10, B: 15, A: 220},
			Background: color.NRGBA{R: 5, G: 5, B: 10, A: 255},
			Text:       color.NRGBA{R: 255, G: 0, B: 60, A: 255},
			Accent:     color.NRGBA{R: 252, G: 238, B: 9, A: 255},
		}
	case Aetheria:
		return Theme{
			Type:       Aetheria,
			Primary:    color.NRGBA{R: 212, G: 175, B: 55, A: 255}, // Cosmic Gold
			Surface:    color.NRGBA{R: 26, G: 11, B: 46, A: 240},   // Nebula Purple
			Background: color.NRGBA{R: 10, G: 2, B: 20, A: 255},
			Text:       color.NRGBA{R: 212, G: 175, B: 55, A: 255},
			Accent:     color.NRGBA{R: 255, G: 255, B: 255, A: 200},
		}
	default:
		return Theme{
			Type:       Dark,
			Primary:    color.NRGBA{R: 0, G: 120, B: 215, A: 255},
			Surface:    color.NRGBA{R: 30, G: 30, B: 30, A: 255},
			Background: color.NRGBA{R: 17, G: 17, B: 17, A: 255},
			Text:       color.NRGBA{R: 255, G: 255, B: 255, A: 255},
			Accent:     color.NRGBA{R: 86, G: 156, B: 214, A: 255},
		}
	}
}
