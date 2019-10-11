package pong

import (
	"log"

	"github.com/chewxy/math32"
	rl "github.com/gen2brain/raylib-go/raylib"
	"github.com/piotrek-szczygiel/raspberry-console/console/controller"
	u "github.com/piotrek-szczygiel/raspberry-console/console/utils"
)

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

	width  float32
	height float32

	players [2]player
	ball    ball
}

func New() *Pong {
	var pong Pong

	w, h := rl.GetScreenWidth(), rl.GetScreenHeight()
	pong.width, pong.height = float32(w), float32(h)

	pong.target = rl.LoadRenderTexture(int32(w), int32(h))
	rl.SetTextureFilter(pong.target.Texture, rl.FilterPoint)

	pong.players[0] = player{
		position: u.Vec2(15, pong.height/2-180/2),
		speed:    u.Vec2(0, 0),
		width:    15,
		height:   180,
	}

	pong.players[1] = player{
		position: u.Vec2(pong.width-25, pong.height/2-180/2),
		speed:    u.Vec2(0, 0),
		width:    15,
		height:   180,
	}

	pong.ball = ball{
		position: u.Vec2(pong.width/2, pong.height/2),
		speed:    computeBallSpeed(u.Vec2(1, 0)),
		radius:   15,
	}

	return &pong
}

func (pong *Pong) Update([]controller.Event) {
	dt := rl.GetFrameTime()
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

func (pong *Pong) updatePositions(dt float32, w bool, s bool, up bool, down bool) {
	const (
		playerSpeed = 700
		ballSpeed   = 850
		friction    = 0.8
	)

	if w {
		pong.players[0].speed.Y = -1
	} else if s {
		pong.players[0].speed.Y = 1
	} else {
		pong.players[0].speed.Y = 0
	}

	if up {
		pong.players[1].speed.Y = -1
	} else if down {
		pong.players[1].speed.Y = 1
	} else {
		pong.players[1].speed.Y = 0
	}

	for i := range pong.players {
		pong.players[i].position.Y += pong.players[i].speed.Y * playerSpeed * dt

		if pong.players[i].position.Y < 10 {
			pong.players[i].position.Y = 10
		} else if pong.players[i].position.Y+float32(pong.players[i].height) > pong.height-10 {
			pong.players[i].position.Y = pong.height - float32(pong.players[i].height) - 10
		}

		playerRect := rl.Rectangle{
			X:      pong.players[i].position.X,
			Y:      pong.players[i].position.Y,
			Width:  float32(pong.players[i].width),
			Height: float32(pong.players[i].height),
		}

		if rl.CheckCollisionCircleRec(pong.ball.position, pong.ball.radius, playerRect) {
			if pong.ball.position.X > 19 && pong.ball.position.X < pong.width-19 {
				pong.ball.speed = computeBallSpeed(rl.Vector2{
					X: -pong.ball.speed.X,
					Y: pong.ball.speed.Y + pong.players[i].speed.Y*friction,
				})

				if pong.ball.position.X < pong.width/2 {
					pong.ball.position.X = pong.players[i].position.X + float32(pong.players[i].width) + pong.ball.radius + 1
				} else {
					pong.ball.position.X = pong.players[i].position.X - pong.ball.radius - 1
				}
			}
		}
	}

	if pong.ball.position.Y < pong.ball.radius {
		pong.ball.position.Y = pong.ball.radius
		pong.ball.speed.Y *= -1
	} else if pong.ball.position.Y > pong.height-pong.ball.radius {
		pong.ball.position.Y = pong.height - pong.ball.radius
		pong.ball.speed.Y *= -1
	}

	pong.ball.position.X += pong.ball.speed.X * ballSpeed * dt
	pong.ball.position.Y += pong.ball.speed.Y * ballSpeed * dt

	if pong.ball.position.X < 15 {
		pong.restart()
	} else if pong.ball.position.X > pong.width-15 {
		pong.restart()
	}
}

func (pong *Pong) restart() {
	modAngle := u.Random(-math32.Pi/8, math32.Pi/8)
	log.Print(modAngle)
	var dirX = pong.ball.speed.X / math32.Abs(pong.ball.speed.X) * -1
	pong.ball = ball{
		position: u.Vec2(pong.width/2, pong.height/2),
		speed:    computeBallSpeed(u.Vec2(math32.Cos(modAngle)*dirX, math32.Sin(modAngle))),
		radius:   15,
	}
	pong.players[0].position = u.Vec2(15, pong.height/2-float32(pong.players[0].height/2))
	pong.players[1].position = u.Vec2(pong.width-25, pong.height/2-float32(pong.players[1].height/2))
}

func computeBallSpeed(v rl.Vector2) rl.Vector2 {
	var length = math32.Sqrt(v.X*v.X + v.Y*v.Y)
	v = u.Vec2(v.X/length, v.Y/length)
	var vXDir = v.X / math32.Abs(v.X)
	var vYDir = v.Y / math32.Abs(v.Y)
	if math32.Abs(v.Y) >= 0.90 {
		v.Y = 0.90 * vYDir
		v.X = math32.Sqrt(1-v.Y*v.Y) * vXDir
	}
	return v
}
