package widgets

import (
	"fmt"

	"gioui.org/layout"
	"gioui.org/unit"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

// DemoSurface is a compile-safe composition target for the Go QuickControls2-style
// primitives so they can be exercised together as one framework demo.
type DemoSurface struct {
	Popup       Popup
	Dialog      Dialog
	Drawer      Drawer
	ToolTip     ToolTip
	Menu        Menu
	WebView     WebView
	ScrollBox   *ScrollView
	ScrollItems []string
	TouchPad    TouchArea
	LastSwipe   string
}

func NewDemoSurface() *DemoSurface {
	d := &DemoSurface{}
	d.Popup = Popup{Visible: true, Title: "Popup", Body: "QuickControls2 baseline popup"}
	d.Dialog = Dialog{Visible: false, Title: "Dialog", Body: "Dialog body"}
	d.Drawer = Drawer{Visible: true, Title: "Drawer"}
	d.ToolTip = ToolTip{Visible: true, Text: "Tooltip baseline", X: 12, Y: 12}
	d.Menu = Menu{Visible: true, Items: []MenuItem{{Label: "Open", Action: "open"}, {Label: "Close", Action: "close"}}}
	d.WebView.Navigate("about:bobui", "<h1>BobUI WebView</h1><br>History and navigation baseline")
	d.ScrollBox = NewScrollView(layout.Vertical)
	d.ScrollBox.VerticalPolicy = ScrollAlwaysOn
	d.TouchPad = TouchArea{
		Label:             "Swipe Demo",
		AllowMouseAsTouch: true,
		MinSwipeDistance:  24,
	}
	d.TouchPad.OnSwipe = func(direction SwipeDirection, distance float32) {
		d.LastSwipe = fmt.Sprintf("%s %.0fpx", direction.String(), distance)
	}
	for i := 1; i <= 40; i++ {
		d.ScrollItems = append(d.ScrollItems, fmt.Sprintf("Scroll item %02d - QuickControls2-style scroll baseline", i))
	}
	return d
}

func (d *DemoSurface) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	// Compose in a simple top-to-bottom structure for verified baseline purposes.
	return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			_, _ = d.Menu.Layout(gtx, th)
			return layout.Dimensions{}
		}),
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			return d.WebView.Layout(gtx, th)
		}),
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			return layout.Inset{Top: unit.Dp(8), Bottom: unit.Dp(8)}.Layout(gtx, func(gtx layout.Context) layout.Dimensions {
				return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
					layout.Rigid(material.Label(mth, unit.Sp(11), fmt.Sprintf("Scroll demo items=%d position=%.2f page=%.2f dragging=%v", len(d.ScrollItems), d.ScrollBox.VerticalBar.Position, d.ScrollBox.VerticalBar.PageSize, d.ScrollBox.VerticalBar.Dragging)).Layout),
					layout.Rigid(layout.Spacer{Height: unit.Dp(6)}.Layout),
					layout.Rigid(func(gtx layout.Context) layout.Dimensions {
						gtx.Constraints.Min.Y = gtx.Dp(unit.Dp(140))
						gtx.Constraints.Max.Y = gtx.Dp(unit.Dp(140))
						return d.ScrollBox.LayoutWithTheme(gtx, th, len(d.ScrollItems), func(gtx layout.Context, i int) layout.Dimensions {
							return layout.Inset{Top: unit.Dp(4), Bottom: unit.Dp(4), Left: unit.Dp(6), Right: unit.Dp(18)}.Layout(gtx, material.Body1(mth, d.ScrollItems[i]).Layout)
						})
					}),
				)
			})
		}),
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			return layout.Inset{Top: unit.Dp(8), Bottom: unit.Dp(8)}.Layout(gtx, func(gtx layout.Context) layout.Dimensions {
				return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
					layout.Rigid(material.Label(mth, unit.Sp(11), fmt.Sprintf("Touch/swipe demo lastSwipe=%s", emptyDemoValue(d.LastSwipe))).Layout),
					layout.Rigid(layout.Spacer{Height: unit.Dp(6)}.Layout),
					layout.Rigid(func(gtx layout.Context) layout.Dimensions {
						return d.TouchPad.Layout(gtx, th)
					}),
				)
			})
		}),
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			return d.Popup.Layout(gtx, th)
		}),
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			_, _ = d.Dialog.Layout(gtx, th)
			return layout.Dimensions{}
		}),
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			return d.ToolTip.Layout(gtx, th)
		}),
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			return d.Drawer.Layout(gtx, th, func(gtx layout.Context) layout.Dimensions {
				return layout.Dimensions{}
			})
		}),
	)
}

func emptyDemoValue(v string) string {
	if v == "" {
		return "none"
	}
	return v
}
