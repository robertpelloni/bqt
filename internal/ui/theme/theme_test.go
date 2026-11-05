package theme

import (
	"image/color"
	"testing"
)

func TestGetTheme(t *testing.T) {
	dark := GetTheme(Dark)
	if dark.Type != Dark {
		t.Errorf("Expected Dark theme type, got %v", dark.Type)
	}

	cyber := GetTheme(Cyberpunk)
	if cyber.Type != Cyberpunk {
		t.Errorf("Expected Cyberpunk theme type, got %v", cyber.Type)
	}
	// Verify Neon Cyan primary for Cyberpunk
	if cyber.Primary != (color.NRGBA{R: 0, G: 240, B: 255, A: 255}) {
		t.Errorf("Unexpected Cyberpunk primary color: %v", cyber.Primary)
	}

	aetheria := GetTheme(Aetheria)
	if aetheria.Type != Aetheria {
		t.Errorf("Expected Aetheria theme type, got %v", aetheria.Type)
	}
}

func TestStyleSheetParsingAndColorResolution(t *testing.T) {
	css := "color: #ff0000; background: #00ff00; border: 1px solid"
	ss := NewStyleSheet(css)
	if ss == nil {
		t.Fatal("Expected NewStyleSheet to return non-nil instance")
	}

	fallback := color.NRGBA{R: 128, G: 128, B: 128, A: 255}

	red := ss.GetColor("color", fallback)
	if red.R != 255 || red.G != 0 || red.B != 0 {
		t.Errorf("Expected red color (255, 0, 0), got %v", red)
	}

	green := ss.GetColor("background", fallback)
	if green.R != 0 || green.G != 255 || green.B != 0 {
		t.Errorf("Expected green color (0, 255, 0), got %v", green)
	}

	missing := ss.GetColor("nonexistent", fallback)
	if missing != fallback {
		t.Errorf("Expected fallback color, got %v", missing)
	}
}
