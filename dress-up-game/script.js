const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');

// Load character base
const character = new Image();
character.src = 'src/profile.jpeg';
character.onload = () => {
    ctx.drawImage(character, 0, 0, canvas.width, canvas.height);
};

function allowDrop(event) {
    event.preventDefault();
}

function drag(event) {
    event.dataTransfer.setData('text', event.target.id);
}

function drop(event) {
    event.preventDefault();
    const data = event.dataTransfer.getData('text');
    const img = document.getElementById(data);
    
    const x = event.offsetX;
    const y = event.offsetY;
    
    const clothingImage = new Image();
    clothingImage.src = img.src;
    clothingImage.onload = () => {
        ctx.drawImage(clothingImage, x, y, 100, 100); // Adjust size and position as needed
    };
}
