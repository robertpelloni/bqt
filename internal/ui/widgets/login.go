package widgets

import (
	"gioui.org/layout"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"gioui.org/unit"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type LoginView struct {
	UserField, PassField widget.Editor
	LoginBtn             widget.Clickable
	Authenticated        bool
}

func (l *LoginView) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	// --- GO NATIVE AUTH RENDER PASS ---
	// High-performance centered login UI natively in Go.
	
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	return layout.Center.Layout(gtx, func(gtx layout.Context) layout.Dimensions {
		return layout.Flex{Axis: layout.Vertical, Alignment: layout.Middle}.Layout(gtx,
			layout.Rigid(func(gtx layout.Context) layout.Dimensions {
				return material.H4(mth, "OMNI.OS GO").Layout(gtx)
			}),
			layout.Rigid(layout.Spacer{Height: unit.Dp(20)}.Layout),
			layout.Rigid(func(gtx layout.Context) layout.Dimensions {
				return material.Editor(mth, &l.UserField, "Username").Layout(gtx)
			}),
			layout.Rigid(func(gtx layout.Context) layout.Dimensions {
				if l.LoginBtn.Clicked(gtx) { l.Authenticated = true }
				return material.Button(mth, &l.LoginBtn, "INITIALIZE").Layout(gtx)
			}),
		)
	})
}
