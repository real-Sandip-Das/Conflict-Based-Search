const canvas = document.getElementById('mapCanvas');
const ctx = canvas.getContext('2d');

let currentStep = 0;
let maxSteps = 0;
let solutionData = null;
let bgCanvas = document.createElement('canvas'); // Offscreen cache

fetch('solution.json')
    .then(response => {
        if (!response.ok) throw new Error("Failed to load JSON");
        return response.json();
    })
    .then(data => {
        solutionData = data;
        solutionData.solution.forEach(path => {
            if (path.length > maxSteps) maxSteps = path.length;
        });

        initMap();
        setInterval(draw, 60); // Draw every 60ms (5x faster)
    })
    .catch(err => {
        alert("Failed to load solution.json! (Remember to run python3 -m http.server)");
        console.error(err);
    });

function initMap() {
    const map = solutionData.map;
    const rows = map.length;
    const cols = map[0].length;
    
    // Set actual canvas resolution to exactly the number of cells (1 pixel = 1 cell)
    canvas.width = cols;
    canvas.height = rows;
    
    // Create offscreen canvas to cache the map background
    bgCanvas.width = cols;
    bgCanvas.height = rows;
    const bgCtx = bgCanvas.getContext('2d');
    
    // Generate ImageData for ultra-fast, C++-like matrix manipulation
    const imgData = bgCtx.createImageData(cols, rows);
    const data = imgData.data;
    
    for (let r = 0; r < rows; r++) {
        for (let c = 0; c < cols; c++) {
            const idx = (r * cols + c) * 4;
            const char = map[r][c];
            if (char === '@') {
                data[idx] = 30; data[idx+1] = 41; data[idx+2] = 59; data[idx+3] = 255; // Slate-800
            } else if (char === 'O') {
                data[idx] = 15; data[idx+1] = 23; data[idx+2] = 42; data[idx+3] = 255; // Slate-900
            } else if (char === 'T') {
                data[idx] = 21; data[idx+1] = 128; data[idx+2] = 61; data[idx+3] = 255; // Green-700 (Trees)
            } else if (char === 'S') {
                data[idx] = 161; data[idx+1] = 98; data[idx+2] = 7; data[idx+3] = 255; // Yellow-700 (Swamp)
            } else if (char === 'W') {
                data[idx] = 59; data[idx+1] = 130; data[idx+2] = 246; data[idx+3] = 255; // Blue-500 (Water)
            } else if (char === 'G') {
                data[idx] = 134; data[idx+1] = 239; data[idx+2] = 172; data[idx+3] = 255; // Green-300 (Grass)
            } else { // '.'
                data[idx] = 226; data[idx+1] = 232; data[idx+2] = 240; data[idx+3] = 255; // Slate-200 (Passable)
            }
        }
    }
    // Commit map to offscreen canvas once
    bgCtx.putImageData(imgData, 0, 0);
}

function draw() {
    // 1. Draw the static map instantly by copying the offscreen canvas
    ctx.drawImage(bgCanvas, 0, 0);
    
    // 2. Draw Agents and their trails
    const colors = ['#ff0000', '#0000ff', '#00ff00', '#ffa500', '#800080', '#00ffff', '#ff00ff'];
    
    solutionData.solution.forEach((path, i) => {
        const color = colors[i % colors.length];
        
        // Draw the trail (past path)
        ctx.fillStyle = color;
        ctx.globalAlpha = 0.4; // Semi-transparent for the trail
        for (let j = 0; j <= Math.min(currentStep, path.length - 1); j++) {
            ctx.fillRect(path[j].x, path[j].y, 1, 1);
        }
        ctx.globalAlpha = 1.0; // Reset opacity
        
        // Draw the agent itself larger (3x3 pixels, centered on its current cell)
        const p = path[Math.min(currentStep, path.length - 1)];
        ctx.fillStyle = color;
        ctx.fillRect(p.x - 1, p.y - 1, 3, 3);
        
        // Add a tiny white dot in the center so it looks nice
        ctx.fillStyle = '#ffffff';
        ctx.fillRect(p.x, p.y, 1, 1);
    });
    
    currentStep++;
    if (currentStep >= maxSteps) currentStep = 0;
}
