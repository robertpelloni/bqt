package widgets

import (
	"image"
	"gioui.org/layout"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/unit"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type TabBar struct {
	Tabs []string
	Active int
}

func (t *TabBar) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	// --- GO NATIVE DOCKING TAB RENDER PASS ---
	// High-performance horizontal tab layout natively in Go.
	
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	return layout.Flex{Axis: layout.Horizontal}.Layout(gtx,
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			for i, tab := range t.Tabs {
				color := th.Surface
				if i == t.Active { color = th.Primary }
				
				// Draw Tab Background
				rect := image.Rect(0, 0, gtx.Dp(unit.Dp(100)), gtx.Dp(unit.Dp(30)))
				stack := clip.Rect(rect).Push(gtx.Ops)
				paint.Fill(gtx.Ops, color)
				stack.Pop()
				
				// Draw Title
				material.Label(mth, unit.Sp(11), tab).Layout(gtx)
			}
			return layout.Dimensions{}
		}),
	)
}
