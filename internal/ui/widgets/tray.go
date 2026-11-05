package widgets

import (
	"image"
	"image/color"

	"gioui.org/layout"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/unit"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

// TrayAction defines a menu action for the system tray.
type TrayAction struct {
	ID       string
	Label    string
	Tooltip  string
	Danger   bool
	OnAction func()
	button   widget.Clickable
}

// TrayIcon provides an interactive system tray component with status indicators
// and popup actions to control framework servers, background workers, and DSP.
type TrayIcon struct {
	MenuVisible bool
	IconClick   widget.Clickable
	Actions     []*TrayAction
	StatusText  string
	StatusColor color.NRGBA
	TooltipText string

	// Server control state callbacks
	OnQuitServer       func()
	OnRestartEventLoop func()
	OnMuteDSP          func()
	OnDisconnectMesh   func()

	// Internal state
	btnQuit       widget.Clickable
	btnRestart    widget.Clickable
	btnMute       widget.Clickable
	btnDisconnect widget.Clickable
	isMuted       bool
	isMeshActive  bool
	serverRunning bool
}

// NewTrayIcon constructs a standard BQt system tray icon with sensible server lifecycle controls.
func NewTrayIcon() *TrayIcon {
	return &TrayIcon{
		StatusText:    "ONLINE",
		StatusColor:   color.NRGBA{R: 0, G: 220, B: 120, A: 255}, // Vibrant green
		TooltipText:   "BQt Server & Core Framework Status (Click for Controls)",
		serverRunning: true,
		isMeshActive:  true,
		isMuted:       false,
	}
}

// LayoutIcon renders the tray icon button inside the taskbar.
func (t *TrayIcon) LayoutIcon(gtx layout.Context, th theme.Theme) layout.Dimensions {
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	if t.IconClick.Clicked(gtx) {
		t.MenuVisible = !t.MenuVisible
	}

	// Tray icon badge rendering
	return t.IconClick.Layout(gtx, func(gtx layout.Context) layout.Dimensions {
		return layout.Inset{Top: unit.Dp(4), Bottom: unit.Dp(4), Left: unit.Dp(8), Right: unit.Dp(8)}.Layout(gtx, func(gtx layout.Context) layout.Dimensions {
			return layout.Flex{Axis: layout.Horizontal, Alignment: layout.Middle, Spacing: layout.SpaceEnd}.Layout(gtx,
				layout.Rigid(func(gtx layout.Context) layout.Dimensions {
					// Draw small circular status indicator light
					size := gtx.Dp(unit.Dp(10))
					circle := clip.Ellipse{
						Min: image.Pt(0, 0),
						Max: image.Pt(size, size),
					}
					paint.FillShape(gtx.Ops, t.StatusColor, circle.Op(gtx.Ops))
					return layout.Dimensions{Size: image.Pt(size+gtx.Dp(unit.Dp(6)), size)}
				}),
				layout.Rigid(func(gtx layout.Context) layout.Dimensions {
					lbl := material.Caption(mth, "BQt Tray")
					lbl.Color = th.Text
					return lbl.Layout(gtx)
				}),
			)
		})
	})
}

// LayoutMenu renders the dropdown tray menu with server lifecycle actions.
func (t *TrayIcon) LayoutMenu(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if !t.MenuVisible {
		return layout.Dimensions{}
	}

	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	// Handle button clicks
	if t.btnMute.Clicked(gtx) {
		t.isMuted = !t.isMuted
		if t.OnMuteDSP != nil {
			t.OnMuteDSP()
		}
	}

	if t.btnDisconnect.Clicked(gtx) {
		t.isMeshActive = !t.isMeshActive
		if t.OnDisconnectMesh != nil {
			t.OnDisconnectMesh()
		}
	}

	if t.btnRestart.Clicked(gtx) {
		if t.OnRestartEventLoop != nil {
			t.OnRestartEventLoop()
		}
	}

	if t.btnQuit.Clicked(gtx) {
		t.serverRunning = false
		t.StatusText = "SHUTTING DOWN"
		t.StatusColor = color.NRGBA{R: 240, G: 50, B: 50, A: 255}
		if t.OnQuitServer != nil {
			t.OnQuitServer()
		}
	}

	menuWidth := gtx.Dp(unit.Dp(240))
	menuHeight := gtx.Dp(unit.Dp(220))

	// Position popup menu near bottom right
	posX := gtx.Constraints.Max.X - menuWidth - gtx.Dp(unit.Dp(12))
	posY := gtx.Constraints.Max.Y - menuHeight - gtx.Dp(unit.Dp(52))
	if posX < 0 {
		posX = 10
	}
	if posY < 0 {
		posY = 10
	}

	menuRect := image.Rect(posX, posY, posX+menuWidth, posY+menuHeight)
	// Render popup background card with border
	paint.FillShape(gtx.Ops, color.NRGBA{R: 28, G: 32, B: 42, A: 245}, clip.Rect(menuRect).Op())
	paint.FillShape(gtx.Ops, th.Primary, clip.Stroke{
		Path: clip.RRect{
			Rect: menuRect,
			SE:   4, SW: 4, NW: 4, NE: 4,
		}.Path(gtx.Ops),
		Width: 1.5,
	}.Op())

	return layout.Inset{
		Top:  unit.Dp(float32(posY) / gtx.Metric.PxPerDp),
		Left: unit.Dp(float32(posX) / gtx.Metric.PxPerDp),
	}.Layout(gtx, func(gtx layout.Context) layout.Dimensions {
		gtx.Constraints.Max.X = menuWidth
		gtx.Constraints.Min.X = menuWidth

		return layout.UniformInset(unit.Dp(12)).Layout(gtx, func(gtx layout.Context) layout.Dimensions {
			return layout.Flex{Axis: layout.Vertical, Spacing: layout.SpaceEvenly}.Layout(gtx,
				layout.Rigid(func(gtx layout.Context) layout.Dimensions {
					title := material.Subtitle2(mth, "BQt System Controller")
					title.Color = th.Text
					return title.Layout(gtx)
				}),
				layout.Rigid(func(gtx layout.Context) layout.Dimensions {
					statusMsg := "State: " + t.StatusText
					if t.isMuted {
						statusMsg += " [Muted]"
					}
					status := material.Caption(mth, statusMsg)
					status.Color = t.StatusColor
					return status.Layout(gtx)
				}),
				layout.Rigid(func(gtx layout.Context) layout.Dimensions {
					muteText := "Mute Audio DSP"
					if t.isMuted {
						muteText = "Unmute Audio DSP"
					}
					return material.Button(mth, &t.btnMute, muteText).Layout(gtx)
				}),
				layout.Rigid(func(gtx layout.Context) layout.Dimensions {
					meshText := "Disconnect P2P Mesh"
					if !t.isMeshActive {
						meshText = "Reconnect P2P Mesh"
					}
					return material.Button(mth, &t.btnDisconnect, meshText).Layout(gtx)
				}),
				layout.Rigid(func(gtx layout.Context) layout.Dimensions {
					return material.Button(mth, &t.btnRestart, "Restart EventLoop").Layout(gtx)
				}),
				layout.Rigid(func(gtx layout.Context) layout.Dimensions {
					btn := material.Button(mth, &t.btnQuit, "Quit BQt Servers")
					btn.Background = color.NRGBA{R: 200, G: 40, B: 40, A: 255}
					return btn.Layout(gtx)
				}),
			)
		})
	})
}
