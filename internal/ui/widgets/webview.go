package widgets

import (
	"gioui.org/layout"
	"gioui.org/widget/material"
	"gioui.org/unit"
	"github.com/robertpelloni/bobui/internal/ui/theme"
	"strings"
)

type WebView struct {
	HTML string
}

func (w *WebView) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	// --- GO NATIVE TINY WEB ENGINE ---
	// Naive HTML tag parsing and rendering natively in the Gio loop.
	
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	lines := strings.Split(w.HTML, "<br>")
	
	list := &layout.List{Axis: layout.Vertical}
	return list.Layout(gtx, len(lines), func(gtx layout.Context, i int) layout.Dimensions {
		line := lines[i]
		
		// Map HTML tags to Gio units
		size := unit.Sp(12)
		if strings.Contains(line, "<h1>") {
			size = unit.Sp(28)
			line = strings.ReplaceAll(line, "<h1>", "")
			line = strings.ReplaceAll(line, "</h1>", "")
		}

		return material.Label(mth, size, line).Layout(gtx)
	})
}
