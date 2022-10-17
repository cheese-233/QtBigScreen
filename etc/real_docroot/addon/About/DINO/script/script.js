const dino = document.querySelector(".dino");
const background = document.querySelector(".background");
const point = document.querySelector(".point");
let isJumping = false;
let tposition = 0;
let position = 0;
let alltime = 0;
function handleKeyUp(event) {
  if (event.keyCode === 32) {
    jump();
  }
}
function handleMouseDown() {
  jump();
}
function jump() {
  if (!isJumping) {
    isJumping = true;
    position = 30;
    tposition = 0;
    let upInterval = setInterval(() => {
      if (position >= 150) {
        clearInterval(upInterval);
        let downInterval = setInterval(() => {
          if (position <= 0) {
            clearInterval(downInterval);
            isJumping = false;
          } else {
            if (tposition > 30) {
              position -= 20;
              dino.style.bottom = position + "px";
            }
            else {
              tposition += 10;
            }
          }
        }, 20);
      } else {
        position += 20;
        dino.style.bottom = position + "px";
      }
    }, 20);
  }
}
let max = 80;
let min = 10;
let end = false;
function createCactus() {
  const cactus = document.createElement("div");
  let cactusPosition = window.innerWidth;
  let randomTime = Math.random() * ((max - min) + min) * 60;

  cactus.classList.add("cactus");
  cactus.style.left = window.innerWidth + "px";
  background.appendChild(cactus);

  let leftInterval = setInterval(() => {
    if (cactusPosition < -60 && !isJumping) {
      clearInterval(leftInterval);
      background.removeChild(cactus);
    } else if (cactusPosition > 0 && cactusPosition < 60 && position < 60) {

      clearInterval(leftInterval);
      end = true;
      document.body.innerHTML = `<h1 class="game-over">Game Over</h1><h2 style="color: #666666">分数:` + alltime + `</h2><input type="button" value="重置" onclick="location.reload();"/><small align="right">小彩蛋 by 何润卿</small>`;
      setTimeout(document.addEventListener("keyup", function () { location.reload(); }), 1000);
    } else {
      cactusPosition -= 10;
      cactus.style.left = cactusPosition + "px";
    }
  }, 20);
  setTimeout(createCactus, randomTime);
}
let isbold = false;
// https://github.com/uxitten/polyfill/blob/master/string.polyfill.js
// https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/padStart
if (!String.prototype.padStart) {
  String.prototype.padStart = function padStart(targetLength, padString) {
    targetLength = targetLength >> 0; //floor if number or convert non-number to 0;
    padString = String((typeof padString !== 'undefined' ? padString : ' '));
    if (this.length > targetLength) {
      return String(this);
    }
    else {
      targetLength = targetLength - this.length;
      if (targetLength > padString.length) {
        padString += padString.repeat(targetLength / padString.length); //append to original to ensure we are longer than needed
      }
      return padString.slice(0, targetLength) + String(this);
    }
  };
}

function addtime() {
  if (end == false) {
    alltime += 1;
    point.innerHTML = (alltime + '').padStart(8, '0') + "&nbsp;";
    if (isbold == true) {
      point.style = "font-weight: 400!important";
      isbold = false;
    }
    if (alltime % 10 == 0) {
      point.style = "font-weight: 900!important";
      isbold = true;
    }
    setTimeout(addtime, 500);
  }
}
createCactus();
addtime();
document.addEventListener("keyup", handleKeyUp);
document.addEventListener("click", handleMouseDown);