package theme

import (
	"image/color"
	"strings"
	"strconv"
)

type StyleSheet struct {
	Properties map[string]string
}

func NewStyleSheet(style string) *StyleSheet {
	ss := &StyleSheet{Properties: make(map[string]string)}
	ss.Parse(style)
	return ss
}

func (ss *StyleSheet) Parse(style string) {
	decls := strings.Split(style, ";")
	for _, decl := range decls {
		parts := strings.Split(decl, ":")
		if len(parts) == 2 {
			key := strings.TrimSpace(strings.ToLower(parts[0]))
			val := strings.TrimSpace(parts[1])
			ss.Properties[key] = val
		}
	}
}

func (ss *StyleSheet) GetColor(key string, fallback color.NRGBA) color.NRGBA {
	val, ok := ss.Properties[key]
	if !ok { return fallback }
	
	// Hex parsing logic natively in Go
	if strings.HasPrefix(val, "#") {
		c, err := strconv.ParseUint(val[1:], 16, 32)
		if err == nil {
			return color.NRGBA{R: uint8(c >> 16), G: uint8(c >> 8), B: uint8(c), A: 255}
		}
	}
	return fallback
}
