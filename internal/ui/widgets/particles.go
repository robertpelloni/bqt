package widgets

import (
	"image"
	"image/color"
	"math"
	"math/rand"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/f32"
)

type Particle struct {
	Pos, Vel f32.Point
	Life     float32
	MaxLife  float32
	Color    color.NRGBA
}

type ParticleSystem struct {
	Particles []Particle
	Active    bool
}

// Update handles the physics simulation in the Go port.
func (ps *ParticleSystem) Update(dt float32) {
	if !ps.Active { return }
	
	for i := len(ps.Particles) - 1; i >= 0; i-- {
		p := &ps.Particles[i]
		p.Life -= dt
		if p.Life <= 0 {
			ps.Particles = append(ps.Particles[:i], ps.Particles[i+1:]...)
			continue
		}
		
		// Apply Gravity Natively in Go
		p.Vel.Y += 9.8 * dt
		p.Pos = p.Pos.Add(p.Vel)
	}
}

func (ps *ParticleSystem) Burst(pos f32.Point, count int) {
	for i := 0; i < count; i++ {
		angle := rand.Float32() * 2 * math.Pi
		speed := rand.Float32()*5 + 1
		ps.Particles = append(ps.Particles, Particle{
			Pos:     pos,
			Vel:     f32.Pt(float32(math.Cos(float64(angle)))*speed, float32(math.Sin(float64(angle)))*speed),
			Life:    1.0 + rand.Float32(),
			MaxLife: 2.0,
			Color:   color.NRGBA{R: 0, G: 240, B: 255, A: 255},
		})
	}
}

func (ps *ParticleSystem) Layout(gtx layout.Context) layout.Dimensions {
	for _, p := range ps.Particles {
		// Render each particle as a GPU Point/Rect
		alpha := uint8((p.Life / p.MaxLife) * 255)
		c := p.Color
		c.A = alpha
		
		rect := f32.Rect(p.Pos.X, p.Pos.Y, p.Pos.X+2, p.Pos.Y+2)
		stack := clip.Rect(rect.Round()).Push(gtx.Ops)
		paint.Fill(gtx.Ops, c)
		stack.Pop()
	}
	return layout.Dimensions{Size: gtx.Constraints.Max}
}
