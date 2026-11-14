package widgets

import (
	"image"

	"gioui.org/layout"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/unit"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

type Shell struct {
	StartMenuVisible bool
	StartBtn         widget.Clickable
	Tray             *TrayIcon
	// OnStartAction receives "dashboard", "tray", or "quit" from the
	// Start Menu. Nil-safe; wired by the Engine.
	OnStartAction      func(action string)
	btnStartDashboard  widget.Clickable
	btnStartTray       widget.Clickable
	btnStartQuit       widget.Clickable
}

func NewShell() *Shell {
	return &Shell{
		Tray: NewTrayIcon(),
	}
}

func (s *Shell) LayoutTaskbar(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if s.Tray == nil {
		s.Tray = NewTrayIcon()
	}

	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	height := gtx.Dp(unit.Dp(48))
	rect := image.Rect(0, gtx.Constraints.Max.Y-height, gtx.Constraints.Max.X, gtx.Constraints.Max.Y)
	paint.FillShape(gtx.Ops, th.Surface, clip.Rect(rect).Op())

	if s.StartBtn.Clicked(gtx) {
		s.StartMenuVisible = !s.StartMenuVisible
	}

	return layout.Flex{Axis: layout.Horizontal, Spacing: layout.SpaceBetween, Alignment: layout.Middle}.Layout(gtx,
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			return material.Button(mth, &s.StartBtn, "START").Layout(gtx)
		}),
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			return layout.Flex{Axis: layout.Horizontal, Alignment: layout.Middle}.Layout(gtx,
				layout.Rigid(func(gtx layout.Context) layout.Dimensions {
					return material.Label(mth, unit.Sp(10), "DSP: SYNC | P2P: ACTIVE  ").Layout(gtx)
				}),
				layout.Rigid(func(gtx layout.Context) layout.Dimensions {
					return s.Tray.LayoutIcon(gtx, th)
				}),
			)
		}),
	)
}

func (s *Shell) LayoutStartMenu(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if !s.StartMenuVisible {
		return layout.Dimensions{}
	}
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface
	act := func(btn *widget.Clickable, action string) {
		if btn.Clicked(gtx) {
			s.StartMenuVisible = false
			if s.OnStartAction != nil {
				s.OnStartAction(action)
			}
		}
	}
	act(&s.btnStartDashboard, "dashboard")
	act(&s.btnStartTray, "tray")
	act(&s.btnStartQuit, "quit")
	rect := image.Rect(10, 10, 260, 190)
	paint.FillShape(gtx.Ops, th.Surface, clip.Rect(rect).Op())
	return layout.Inset{Top: unit.Dp(10), Left: unit.Dp(10)}.Layout(gtx, func(gtx layout.Context) layout.Dimensions {
		return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
			layout.Rigid(material.Label(mth, unit.Sp(11), "BQt Applications — everything lives on the dashboard").Layout),
			layout.Rigid(material.Button(mth, &s.btnStartDashboard, "Open Dashboard (all systems)").Layout),
			layout.Rigid(material.Button(mth, &s.btnStartTray, "Tray Controls (mute / mesh / restart)").Layout),
			layout.Rigid(material.Button(mth, &s.btnStartQuit, "Quit BQt Servers").Layout),
		)
	})
}
