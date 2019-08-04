local LBorder = nil
local RBorder = nil
local TBorder = nil
local Ball = nil
local Bat = nil
local cPixel = nil

local function GetTextLength(text, scale, invert)
    invert = invert or false
    return invert and ((text:len() * 8 * scale) * -1) or text:len() * 8 * scale
end

local function DrawBorders()
    for x = 0, ((Screen.x / TBorder.x) - 1) do
        Draw.Sprite(TBorder.x * x + 4, 0, TBorder) -- top border
        for y = 0, ((Screen.y / LBorder.y) - 1) do
            Draw.Sprite(0, LBorder.y * y, LBorder) -- left border
            Draw.Sprite(Screen.x - RBorder.x, RBorder.y * y, RBorder) -- right border
        end
    end
end

local function DrawBall(x, y)
    Draw.Sprite(x, y, Ball)
end

local function DrawBat(x, offset)
    local rMouseMax = x + Bat.x / 2
    x = x - Bat.x / 2
    local lMax = LBorder.x
    local rMax = Screen.x - RBorder.x
    if x < lMax then
        Draw.Sprite(lMax, Screen.y - offset, Bat)
    elseif rMouseMax > rMax then
        Draw.Sprite(rMax - Bat.x, Screen.y - offset, Bat)
    else
        Draw.Sprite(x, Screen.y - offset, Bat)
    end
end

function OnUserCreate()
    LBorder = Sprite.new("Sprites/LeftBorder.jpg")
    RBorder = Sprite.new("Sprites/RightBorder.jpg")
    TBorder = Sprite.new("Sprites/TopBorder.jpg")
    Ball = Sprite.new("Sprites/Ball.jpg")
    Bat = Sprite.new("Sprites/Bat.jpg")
    cPixel = pixel.new(0, 0, 0, 255)
end

function OnUserUpdate(fElapsedTime)
    Screen.Clear(cPixel)
    Draw.SetPixelMode(2)
    --for x = 0, ((Screen.x / tile.x) - 1) do
        --Draw.Sprite(tile.x * x, 0, tile)
        --Draw.Sprite(tile.x * x, Screen.y - tile.y, tile)
    --end
    DrawBorders()
    DrawBall(100, 100)
    DrawBat(IO.mouseX, 15)

    --Draw.String(30, 0, string.format("x = %s, y = %s, screeny = %s", tile.x, tile.y, Screen.y), cPixel, 1)
end