package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestTabBarStateAndLayout(t *testing.T) {
	tabBar := &TabBar{
		Tabs:   []string{"Editor", "Terminal", "Output"},
		Active: 0,
	}

	if tabBar.ActiveTitle() != "Editor" {
		t.Fatalf("expected active title 'Editor', got %q", tabBar.ActiveTitle())
	}

	tabBar.ClickTab(1)
	// Click queues an event on button 1

	var ops op.Ops
	gtx := layout.Context{
		Ops: &ops,
		Constraints: layout.Exact(image.Point{
			X: 400,
			Y: 50,
		}),
		Metric: unit.Metric{PxPerDp: 1.0, PxPerSp: 1.0},
	}

	th := theme.GetTheme(theme.Dark)
	dims := tabBar.Layout(gtx, th)

	if dims.Size.X <= 0 || dims.Size.Y <= 0 {
		t.Fatalf("expected non-zero layout dimensions, got %+v", dims.Size)
	}

	// Layout process processes the clicked event, updating active to 1
	if tabBar.Active != 1 {
		t.Fatalf("expected active tab index 1 after click, got %d", tabBar.Active)
	}
	if tabBar.ActiveTitle() != "Terminal" {
		t.Fatalf("expected active title 'Terminal', got %q", tabBar.ActiveTitle())
	}
}

func TestTabBarEdgeCases(t *testing.T) {
	var nilTabBar *TabBar
	if nilTabBar.ActiveTitle() != "" {
		t.Errorf("expected empty string for nil TabBar, got %q", nilTabBar.ActiveTitle())
	}

	emptyTabBar := &TabBar{Tabs: []string{}}
	if emptyTabBar.ActiveTitle() != "" {
		t.Errorf("expected empty string for empty tabs, got %q", emptyTabBar.ActiveTitle())
	}

	var ops op.Ops
	gtx := layout.Context{
		Ops:         &ops,
		Constraints: layout.Exact(image.Point{X: 400, Y: 50}),
		Metric:      unit.Metric{PxPerDp: 1.0, PxPerSp: 1.0},
	}
	th := theme.GetTheme(theme.Dark)

	emptyDims := emptyTabBar.Layout(gtx, th)
	if emptyDims.Size.X != 0 || emptyDims.Size.Y != 0 {
		t.Errorf("expected zero dimensions for empty tab bar, got %v", emptyDims.Size)
	}

	oobTabBar := &TabBar{
		Tabs:   []string{"Home", "Settings"},
		Active: 99,
	}
	if oobTabBar.ActiveTitle() != "Home" {
		t.Errorf("expected OOB active index to clamp to 'Home', got %q", oobTabBar.ActiveTitle())
	}

	negTabBar := &TabBar{
		Tabs:   []string{"Home", "Settings"},
		Active: -5,
	}
	if negTabBar.ActiveTitle() != "Home" {
		t.Errorf("expected negative active index to clamp to 'Home', got %q", negTabBar.ActiveTitle())
	}
}
