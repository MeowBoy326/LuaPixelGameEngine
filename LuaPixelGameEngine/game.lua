local sX = 0
local sY = 0
local p = nil
local cPixel = nil
local posx = 0
local posy = math.random(0, sY)
local msg = string.format("key %s held = %s", Key.V, IO.Key(Key.V).held)
local tile = nil

local function GetTextLength(text, scale, invert)
    invert = invert or false
    return invert and ((text:len() * 8 * scale) * -1) or text:len() * 8 * scale
end

function OnUserCreate()
    p = pixel.new(math.random(256), math.random(256), math.random(256), 255)
    cPixel = pixel.new(0, 0, 0, 255)
    sX = Screen.x
    sY = Screen.y
    posx = GetTextLength(msg, 1, true)
    tile = Sprite.new("Sprites/border.jpg")
end

function OnUserUpdate(fElapsedTime)
    if posx >= sX then
        posx = GetTextLength(msg, 1, true)
        posy = math.random(0, sY)
        p.r = math.random(0, 255)
        p.g = math.random(0, 255)
        p.b = math.random(0, 255)
    end

    for i = 0, sX do
        msg = string.format("mouse held = %s", IO.mouseWheel)
        Screen.Clear(cPixel)
        posx = posx + fElapsedTime * 0.2

        Draw.String(math.floor(posx), posy + 1, msg, p, 1)
        Draw.SetPixelMode(2)
        Draw.Sprite(0, 0, tile)
        --Draw.Line(math.floor(posx), posy + 1, IO.mouseX, IO.mouseY, p, 0xFFFFFFFF)
    end
end