
// let c = document.getElementById("myCanvas");
// let ctx = c.getContext("2d");
// let img = document.getElementById("scream");
// ctx.drawImage(img, 10, 10);


// const image = new Image(200,100);
// image.src = 'gundog.jpg';
// document.body.appendChild( image );


// function make_base()
// {
//     // ctx.drawImage( image, 0, 0, 50, 50 );
//     // ctx.lineWidth = 5;
//     // ctx.strokeRect( 20, 20, image.width, image.height ); // Draw a border around image...
//     // image.onload = function(){
//     //     ctx.drawImage( image, 20, 20, 50, 50 );
//     // }
// }
//Why no image?
// The function should be onloaded


// Method 1 to create a canvas
// let canvases = document.getElementsByTagName("canvas");
// let canvas1 = canvases[0];

// Method 2 to create a canvas
let canvas = document.getElementById('myCanvas');
let ctx = canvas.getContext('2d');
ctx.canvas.width  = window.innerWidth;
ctx.canvas.height = window.innerHeight;
let cWidth = canvas.width;
let cHeight = canvas.height;


let honey = new Image();
honey.src = "honey.png";
honey.xpos = 500;
honey.ypos = 300;
let beeflip = new Image();
beeflip.src = "beeFlip.png";

// Bee Objects
let bee1 = {};
bee1.xpos = 50;
bee1.ypos = 100;
bee1.img = new Image();
bee1.src = "bee.png";
let bee2 = {};
bee2.xpos = 400;
bee2.ypos = 420;
bee2.img = new Image();
bee2.img.src = "bee.png";
let bee3 = {};
bee3.xpos = 700;
bee3.ypos = 300;
bee3.img = new Image();
bee3.img.src = "bee.png";
let bee4 = {};
bee4.xpos = 580;
bee4.ypos = 230;
bee4.img = new Image();
bee4.img.src = "bee.png";
let bee5 = {};
bee5.xpos = 640;
bee5.ypos = 30;
bee5.img = new Image();
bee5.img.src = "bee.png";
let bees = [ bee1, bee2, bee3, bee4, bee5 ];



function main() {
    window.requestAnimationFrame( animate );
}
window.onload = main;

function honeyMove( e ) {
    honey.xpos = e.x -75;
    honey.ypos = e.y -75;
}
document.onmousemove = honeyMove;


function beeMove( e ) {
    let xcount = 5;
    let ycount = 5;
    for ( let i = 0; i < bees.length; i ++ ) {
        if ( bees[i].xpos < honey.xpos ) {
            bees[i].xpos += xcount;
        } else if ( bees[i] > honey.xpos ) {
            bees[i].xpos -= xcount;
        }
        xcount++;
    }
    for ( let i = 0; i < bees.length; i ++ ) {
        if ( bees[i].ypos < honey.ypos ) {
            bees[i].ypos += ycount;
        } else if ( bees[i] > honey.ypos ) {
            bees[i].ypos -= ycount;
        }
        ycount++;
    }
}
document.onload = beeMove;


function erase() {
    ctx.fillStyle = '#FFFFFF';
    ctx.fillRect(0,0, cWidth, cHeight );
}


let isOver;

function gameOver( e ) {
    for ( let i = 0; i < bees.length; i ++ ) {
        if ( bees[i].xpos === honey.xpos && bees[i].ypos === honey.ypos ) {
            isOver = true;
        }
    }
}
document.onload = gameOver;


let goingRight;
let goingDown;
let overImage = new Image();
overImage.src = "gundog.jpg";


function animate() {
    erase();
    ctx.drawImage( honey, honey.xpos, honey.ypos, 150, 150 );
    beeMove();
    gameOver();
    if ( isOver ) {
        ctx.drawImage(overImage, 0, 0, canvas.width, canvas.height)
    } else {
        for ( let i = 0; i < bees.length; i++ ) {
            ctx.drawImage( bees[i].img, bees[i].xpos, bees[i].ypos, 150, 150 );
            if ( goingRight ) {
                bees[i].xpos += 5;
            } else {
                bees[i].xpos -= 5;
            }
            if ( goingDown ) {
                bees[i].ypos += 5;
            } else {
                bees[i].ypos -= 5
            }
            if ( bees[i].xpos +150 >= cWidth ) {
                goingRight = false;
                bees[i].src = "beeFlip.png";
            } else if ( bees[i].xpos <= 0 ) {
                goingRight = true;
                bees[i].src = "bee.png";
            }
            if ( bees[i].ypos +150 > cHeight ) {
                goingDown = false;
            } else if ( bees[i].ypos <= 0) {
                goingDown = true;
            }
        }
        window.requestAnimationFrame( animate ); // <- 60 hz
    }
}








