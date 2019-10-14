package spaceships

import (
	"sort"

	"github.com/chewxy/math32"
	rl "github.com/gen2brain/raylib-go/raylib"
	"github.com/piotrek-szczygiel/raspberry-console/console/controller"
	u "github.com/piotrek-szczygiel/raspberry-console/console/utils"
)

type Spaceships struct {
	ship    ship
	columns []column

	pressed        [3]bool
	renderDistance float32

	target rl.RenderTexture2D

	width  float32
	height float32
	length float32
}

func New() *Spaceships {
	var demo Spaceships

	demo.width = 40
	demo.height = 30
	demo.length = 10000

	demo.ship = newShip(5, 5, 0, 60)
	demo.columns = generateColumns(int(demo.length/10), demo.length, demo.width, demo.height)

	sort.Slice(demo.columns[:], func(i, j int) bool {
		return demo.columns[i].position.X < demo.columns[j].position.X
	})

	demo.renderDistance = 500

	demo.target = rl.LoadRenderTexture(int32(rl.GetScreenWidth()), int32(rl.GetScreenHeight()))
	rl.SetTextureFilter(demo.target.Texture, rl.FilterAnisotropic4x)

	return &demo
}

func (s *Spaceships) Update(events []controller.Event) {
	for _, event := range events {
		switch event.Type {
		case controller.Down:
			s.pressed[event.Key] = true
		case controller.Up:
			s.pressed[event.Key] = false
		}
	}

	dt := rl.GetFrameTime()

	s.ship.forward = rl.IsKeyDown(rl.KeyW) || s.pressed[controller.Middle]
	s.ship.back = rl.IsKeyDown(rl.KeyS)
	s.ship.left = rl.IsKeyDown(rl.KeyA) || s.pressed[controller.Left]
	s.ship.right = rl.IsKeyDown(rl.KeyD) || s.pressed[controller.Right]
	s.ship.up = rl.IsKeyDown(rl.KeyUp)
	s.ship.down = rl.IsKeyDown(rl.KeyDown)

	s.ship.update(dt, s.columns, s.length, s.width, s.height/2)
}

func (s *Spaceships) Draw() {
	rl.BeginTextureMode(s.target)
	rl.ClearBackground(rl.Black)
	rl.BeginMode3D(s.ship.getCamera())

	rl.DrawPlane(u.Vec3(s.length/2, 0, 0), u.Vec2(s.length, s.width+2), rl.DarkGray)

	columnsDrew := 0
	var drewFirstColumn bool
	for i := range s.columns {
		if math32.Abs(s.columns[i].position.X-s.ship.position.X) < s.renderDistance {
			drewFirstColumn = true
			s.columns[i].draw()
			columnsDrew++
		} else if drewFirstColumn {
			break
		}

		if columnsDrew%100 == 0 {
			rl.EndMode3D()
			rl.BeginMode3D(s.ship.getCamera())
		}
	}

	rl.EndMode3D()
	rl.EndTextureMode()
}

func (s *Spaceships) GetTarget() rl.RenderTexture2D {
	return s.target
}
