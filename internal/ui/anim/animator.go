package anim

import (
	"sync"
	"time"
)

type Animation struct {
	StartVal, EndVal float32
	StartTime       time.Time
	Duration        time.Duration
	OnUpdate        func(float32)
	Done            bool
}

type Animator struct {
	mu         sync.Mutex
	animations []*Animation
}

var (
	animInstance *Animator
	animOnce     sync.Once
)

func GetAnimator() *Animator {
	animOnce.Do(func() {
		animInstance = &Animator{}
	})
	return animInstance
}

// AnimateTo starts a native Go property morph.
func (a *Animator) AnimateTo(start, end float32, dur time.Duration, update func(float32)) {
	a.mu.Lock()
	defer a.mu.Unlock()
	a.animations = append(a.animations, &Animation{
		StartVal:  start,
		EndVal:    end,
		StartTime: time.Now(),
		Duration:  dur,
		OnUpdate:  update,
	})
}

// Process updates all active Go animations.
func (a *Animator) Process() {
	a.mu.Lock()
	defer a.mu.Unlock()

	for i := len(a.animations) - 1; i >= 0; i-- {
		anim := a.animations[i]
		elapsed := time.Since(anim.StartTime)
		progress := float32(elapsed.Seconds() / anim.Duration.Seconds())

		if progress >= 1.0 {
			anim.OnUpdate(anim.EndVal)
			a.animations = append(a.animations[:i], a.animations[i+1:]...)
			continue
		}

		// Ease-Out Cubic Interpolation
		t := progress - 1.0
		eased := t*t*t + 1.0
		val := anim.StartVal + (anim.EndVal-anim.StartVal)*eased
		anim.OnUpdate(val)
	}
}
