package widgets

import (
	"gioui.org/layout"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"gioui.org/unit"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type CodeEditor struct {
	Editor widget.Editor
}

func (e *CodeEditor) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	// --- GO NATIVE EDITOR RENDER PASS ---
	// High-performance multi-line text layout natively in Go.
	
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	editor := material.Editor(mth, &e.Editor, "System Kernel Code...")
	editor.TextSize = unit.Sp(13)
	editor.Font.Variant = "Mono"

	return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
		layout.Flexed(1, func(gtx layout.Context) layout.Dimensions {
			return editor.Layout(gtx)
		}),
	)
}
