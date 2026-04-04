package widgets

import (
	"gioui.org/layout"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

// DemoSurface is a compile-safe composition target for the Go QuickControls2-style
// primitives so they can be exercised together as one framework demo.
type DemoSurface struct {
	Popup     Popup
	Dialog    Dialog
	Drawer    Drawer
	ToolTip   ToolTip
	Menu      Menu
	WebView   WebView
	ScrollBox *ScrollView
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
	return d
}

func (d *DemoSurface) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
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
