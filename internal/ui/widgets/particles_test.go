package widgets

import (
	"image"
	"testing"

	"gioui.org/f32"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
)

func TestParticleSystemLifecycleAndLayout(t *testing.T) {
	ps := &ParticleSystem{
		Active: true,
	}

	// Burst 20 particles at (50, 50)
	ps.Burst(f32.Pt(50, 50), 20)
	if len(ps.Particles) != 20 {
		t.Fatalf("expected 20 particles created by Burst, got %d", len(ps.Particles))
	}

	// Step forward physics
	initialY := ps.Particles[0].Pos.Y
	ps.Update(0.1)

	if ps.Particles[0].Pos.Y == initialY && ps.Particles[0].Vel.Y == 0 {
		t.Fatal("expected particle position and velocity to change under gravity")
	}

	// Step forward to expire all particles
	ps.Update(5.0)
	if len(ps.Particles) != 0 {
		t.Fatalf("expected 0 particles after expiration, got %d", len(ps.Particles))
	}

	// Test layout rendering
	ps.Burst(f32.Pt(100, 100), 5)

	var ops op.Ops
	gtx := layout.Context{
		Ops: &ops,
		Constraints: layout.Exact(image.Point{
			X: 400,
			Y: 300,
		}),
		Metric: unit.Metric{PxPerDp: 1.0, PxPerSp: 1.0},
	}

	dims := ps.Layout(gtx)
	if dims.Size.X != 400 || dims.Size.Y != 300 {
		t.Fatalf("expected layout dims (400, 300), got %+v", dims.Size)
	}
}
