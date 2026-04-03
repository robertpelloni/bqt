package widgets

import (
	"gioui.org/layout"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"gioui.org/unit"
	"gioui.org/op/paint"
	"gioui.org/f32"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type LoginView struct {
	UserField, PassField widget.Editor
	LoginBtn             widget.Clickable
	Authenticated        bool
}

func (l *LoginView) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	// --- GO NATIVE AETHERIA AUTH RENDER ---
	// Celestial background and golden button logic.
	
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	// Render Nebula Background simulation
	paint.Fill(gtx.Ops, th.Background)

	return layout.Center.Layout(gtx, func(gtx layout.Context) layout.Dimensions {
		return layout.Flex{Axis: layout.Vertical, Alignment: layout.Middle}.Layout(gtx,
			layout.Rigid(func(gtx layout.Context) layout.Dimensions {
				lbl := material.H4(mth, "OMNI.OS GO")
				lbl.Color = th.Primary // Gold
				return lbl.Layout(gtx)
			}),
			layout.Rigid(func(gtx layout.Context) layout.Dimensions {
				if l.LoginBtn.Clicked(gtx) { l.Authenticated = true }
				btn := material.Button(mth, &l.LoginBtn, "INITIALIZE KERNEL")
				btn.Background = th.Primary
				btn.Color = th.Background
				return btn.Layout(gtx)
			}),
		)
	})
}
