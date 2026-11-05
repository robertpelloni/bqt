package anim

import (
	"testing"
	"time"
)

func TestAnimatorSingleton(t *testing.T) {
	a := GetAnimator()
	b := GetAnimator()
	if a == nil || b == nil {
		t.Fatal("Expected GetAnimator to return non-nil instance")
	}
	if a != b {
		t.Fatal("Expected GetAnimator to return the same singleton instance")
	}
}

func TestAnimatorProcessUpdatesAndFinishes(t *testing.T) {
	animator := GetAnimator()

	var updatedVal float32
	animator.AnimateTo(0.0, 100.0, 20*time.Millisecond, func(v float32) {
		updatedVal = v
	})

	// Process immediately - progress should be close to start
	animator.Process()
	if updatedVal < 0.0 || updatedVal > 100.0 {
		t.Errorf("Unexpected interpolated value: %f", updatedVal)
	}

	// Sleep past the duration to ensure completion
	time.Sleep(30 * time.Millisecond)
	animator.Process()

	// Should reach exactly EndVal (100.0) upon completion
	if updatedVal != 100.0 {
		t.Errorf("Expected updatedVal to reach end value 100.0, got %f", updatedVal)
	}

	// Active animation slice should be cleared
	animator.mu.Lock()
	count := len(animator.animations)
	animator.mu.Unlock()
	if count != 0 {
		t.Errorf("Expected 0 active animations after completion, got %d", count)
	}
}
