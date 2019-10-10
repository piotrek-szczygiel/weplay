package pong

import (
	"math"

	rl "github.com/gen2brain/raylib-go/raylib"
	"github.com/piotrek-szczygiel/raspberry-console/console/controller"
)

const playerSpeed = 425
const ballSpeed = 450

type player struct {
	position rl.Vector2
	speed    rl.Vector2
	width    int32
	height   int32
}

type ball struct {
	position rl.Vector2
	speed    rl.Vector2
	radius   float32
}

type Pong struct {
	target rl.RenderTexture2D

	width  int32
	height int32

	players [2]player
	ball    ball
}

func New() *Pong {
	var pong Pong

	pong.width = 1920
	pong.height = 1080
	pong.target = rl.LoadRenderTexture(pong.width, pong.height)
	rl.SetTextureFilter(pong.target.Texture, rl.FilterPoint)

	pong.players[0] = player{
		position: rl.Vector2{X: 15, Y: 10},
		width:    15,
		height:   150,
	}

	pong.players[1] = player{
		position: rl.Vector2{X: float32(pong.width - 25), Y: 10},
		width:    15,
		height:   150,
	}

	pong.ball = ball{
		position: rl.Vector2{X: float32(pong.width) / 2, Y: float32(pong.height) / 2},
		speed:    rl.Vector2{X: ballSpeed, Y: ballSpeed},
		radius:   15,
	}

	return &pong
}

func (pong *Pong) Update([]controller.Event) {
	dt := float64(rl.GetFrameTime())
	var w, s, up, down bool

	if rl.IsKeyDown(rl.KeyW) {
		w = true
	}

	if rl.IsKeyDown(rl.KeyS) {
		s = true
	}

	if rl.IsKeyDown(rl.KeyUp) {
		up = true
	}

	if rl.IsKeyDown(rl.KeyDown) {
		down = true
	}

	pong.updatePositions(dt, w, s, up, down)
}

func (pong *Pong) Draw() {
	rl.BeginTextureMode(pong.target)
	rl.ClearBackground(rl.Black)

	for _, player := range pong.players {
		rl.DrawRectangle(int32(player.position.X), int32(player.position.Y), player.width, player.height, rl.White)
	}

	rl.DrawCircle(int32(pong.ball.position.X), int32(pong.ball.position.Y), pong.ball.radius, rl.RayWhite)
	rl.EndTextureMode()
}

func (pong *Pong) GetTarget() rl.RenderTexture2D {
	return pong.target
}

func (pong *Pong) updatePositions(dt float64, w bool, s bool, up bool, down bool) {
	if w {
		pong.players[0].speed.Y = -playerSpeed
	} else if s {
		pong.players[0].speed.Y = playerSpeed
	} else {
		pong.players[0].speed.Y = 0
	}

	if up {
		pong.players[1].speed.Y = -playerSpeed
	} else if down {
		pong.players[1].speed.Y = playerSpeed
	} else {
		pong.players[1].speed.Y = 0
	}

	for i := range pong.players {
		pong.players[i].position.Y += pong.players[i].speed.Y * float32(dt)

		if pong.players[i].position.Y < 10 {
			pong.players[i].position.Y = 10
		} else if int32(pong.players[i].position.Y)+pong.players[i].height > pong.height-10 {
			pong.players[i].position.Y = float32(pong.height) - float32(pong.players[i].height) - 10
		}

		playerRect := rl.Rectangle{
			X:      pong.players[i].position.X,
			Y:      pong.players[i].position.Y,
			Width:  float32(pong.players[i].width),
			Height: float32(pong.players[i].height),
		}

		if rl.CheckCollisionCircleRec(pong.ball.position, pong.ball.radius, playerRect) {
			pong.ball.speed = normalize(rl.Vector2{
				X: -pong.ball.speed.X,
				Y: pong.ball.speed.Y + pong.players[i].speed.Y,
			})
			pong.ball.speed = rl.Vector2{X: pong.ball.speed.X * ballSpeed, Y: pong.ball.speed.Y * ballSpeed}
		}
	}

	pong.ball.position.X += pong.ball.speed.X * float32(dt)
	pong.ball.position.Y += pong.ball.speed.Y * float32(dt)

	if pong.ball.position.Y < pong.ball.radius {
		pong.ball.position.Y = pong.ball.radius
		pong.ball.speed.Y *= -1
	} else if pong.ball.position.Y > float32(pong.height)-pong.ball.radius {
		pong.ball.position.Y = float32(pong.height) - pong.ball.radius
		pong.ball.speed.Y *= -1
	}
}

func normalize(v rl.Vector2) rl.Vector2 {
	var length = float32(math.Sqrt(float64(v.X*v.X + v.Y*v.Y)))
	return rl.Vector2{X: v.X / length, Y: v.Y / length}
}
