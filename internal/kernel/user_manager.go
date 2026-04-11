package kernel

import (
	"sync"
)

type User struct {
	ID          string `json:"id"`
	Name        string `json:"name"`
	CursorColor string `json:"color"`
}

type UserManager struct {
	mu            sync.RWMutex
	users         map[string]*User
	deviceToUser  map[string]string // DeviceID -> UserID
}

var (
	userInstance *UserManager
	userOnce     sync.Once
)

func GetUserManager() *UserManager {
	userOnce.Do(func() {
		userInstance = &UserManager{
			users:        make(map[string]*User),
			deviceToUser: make(map[string]string),
		}
	})
	return userInstance
}

func (um *UserManager) CreateUser(id, name, color string) {
	um.mu.Lock()
	defer um.mu.Unlock()
	um.users[id] = &User{ID: id, Name: name, CursorColor: color}
}

func (um *UserManager) AssignDevice(deviceId, userId string) {
	um.mu.Lock()
	defer um.mu.Unlock()
	um.deviceToUser[deviceId] = userId
}

func (um *UserManager) GetUserForDevice(deviceId string) *User {
	um.mu.RLock()
	defer um.mu.RUnlock()
	if userId, exists := um.deviceToUser[deviceId]; exists {
		return um.users[userId]
	}
	return nil
}
