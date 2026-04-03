package kernel

import (
	"sync"
)

type DeviceType string

const (
	Mouse    DeviceType = "mouse"
	Keyboard DeviceType = "keyboard"
	Touch    DeviceType = "touch"
)

type DeviceState struct {
	ID         string     `json:"id"`
	Name       string     `json:"name"`
	Type       DeviceType `json:"type"`
	X, Y       float64    `json:"x"`
	OwnerID    string     `json:"owner_id"`
}

// InputManager manages all multi-cursor states in the Go Kernel.
type InputManager struct {
	mu      sync.RWMutex
	devices map[string]*DeviceState
}

var (
	inputInstance *InputManager
	inputOnce     sync.Once
)

// Instance returns the InputManager singleton.
func GetInputManager() *InputManager {
	inputOnce.Do(func() {
		inputInstance = &InputManager{
			devices: make(map[string]*DeviceState),
		}
	})
	return inputInstance
}

// RegisterDevice adds a new physical/virtual device to the Go kernel.
func (im *InputManager) RegisterDevice(id, name string, devType DeviceType) {
	im.mu.Lock()
	defer im.mu.Unlock()

	if _, exists := im.devices[id]; !exists {
		im.devices[id] = &DeviceState{
			ID:   id,
			Name: name,
			Type: devType,
		}
	}
}

// UpdateCursor updates the position of a specific hardware device ID.
func (im *InputManager) UpdateCursor(id string, x, y float64) {
	im.mu.Lock()
	defer im.mu.Unlock()

	if dev, exists := im.devices[id]; exists {
		dev.X = x
		dev.Y = y
	}
}

// GetDevices returns a snapshot of all active cursors for the UI render loop.
func (im *InputManager) GetDevices() []DeviceState {
	im.mu.RLock()
	defer im.mu.RUnlock()

	snapshot := make([]DeviceState, 0, len(im.devices))
	for _, dev := range im.devices {
		snapshot = append(snapshot, *dev)
	}
	return snapshot
}
