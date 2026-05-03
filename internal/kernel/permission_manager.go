package kernel

import (
	"sync"
)

type PermissionManager struct {
	// ObjectPointerString -> UserID
	locks sync.Map
}

var (
	permInstance *PermissionManager
	permOnce     sync.Once
)

func GetPermissionManager() *PermissionManager {
	permOnce.Do(func() {
		permInstance = &PermissionManager{}
	})
	return permInstance
}

// TryLock attempts to grab an interaction lock for a specific device.
func (pm *PermissionManager) TryLock(deviceID string, target string) bool {
	actual, loaded := pm.locks.LoadOrStore(target, deviceID)
	if !loaded {
		return true // Lock acquired
	}
	return actual == deviceID // True if we already own it
}

// Unlock releases the interaction lock.
func (pm *PermissionManager) Unlock(deviceID string, target string) {
	if actual, ok := pm.locks.Load(target); ok && actual == deviceID {
		pm.locks.Delete(target)
	}
}

// CanInteract checks if the device is permitted to touch the target.
func (pm *PermissionManager) CanInteract(deviceID string, target string) bool {
	actual, ok := pm.locks.Load(target)
	if !ok {
		return true // No lock, everyone can touch
	}
	return actual == deviceID
}
