package widgets

import (
	"image"
	"testing"
	"time"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestCalendarLayout(t *testing.T) {
	cal := &Calendar{}

	var ops op.Ops
	gtx := layout.Context{
		Ops: &ops,
		Constraints: layout.Exact(image.Point{
			X: 400,
			Y: 300,
		}),
		Metric: unit.Metric{PxPerDp: 1.0, PxPerSp: 1.0},
	}

	th := theme.GetTheme(theme.Dark)

	// Zero date falls back to time.Now()
	dims := cal.Layout(gtx, th)
	if cal.CurrentDate.IsZero() {
		t.Fatal("expected CurrentDate to be populated from fallback time.Now()")
	}
	if dims.Size.X <= 0 || dims.Size.Y <= 0 {
		t.Fatalf("expected non-zero layout dimensions, got %+v", dims.Size)
	}

	// Explicit date test
	calExplicit := &Calendar{
		CurrentDate: time.Date(2026, time.November, 6, 12, 0, 0, 0, time.Local),
	}
	dimsExplicit := calExplicit.Layout(gtx, th)
	if dimsExplicit.Size.X <= 0 || dimsExplicit.Size.Y <= 0 {
		t.Fatalf("expected non-zero layout dimensions for explicit date, got %+v", dimsExplicit.Size)
	}
}
