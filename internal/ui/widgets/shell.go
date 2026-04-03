package widgets

import (
	"image"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/unit"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type Shell struct {
	StartMenuVisible bool
	RecordBtn, PlayBtn widget.Clickable
}

func (s *Shell) LayoutTaskbar(gtx layout.Context, th theme.Theme) layout.Dimensions {
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	height := gtx.Dp(unit.Dp(48))
	rect := image.Rect(0, gtx.Constraints.Max.Y-height, gtx.Constraints.Max.X, gtx.Constraints.Max.Y)
	
	defer clip.Rect(rect).Push(gtx.Ops).Pop()
	paint.Fill(gtx.Ops, th.Surface)

	return layout.Flex{Axis: layout.Horizontal, Alignment: layout.Middle}.Layout(gtx,
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			return material.Button(mth, &s.RecordBtn, "REC").Layout(gtx)
		}),
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			return material.Button(mth, &s.PlayBtn, "PLAY").Layout(gtx)
		}),
	)
}

func (s *Shell) LayoutStartMenu(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if !s.StartMenuVisible { return layout.Dimensions{} }
	// ... (menu rendering)
	return layout.Dimensions{}
}
