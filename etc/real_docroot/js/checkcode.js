var show_num = [];
draw(show_num);
function dj() {
    document.getElementById('checktext').value = "";
    draw(show_num);
}
function sublim() {
    var val = document.getElementById("checktext").value;
    var num = show_num.join("");
    if (val == '') {
        alert('请输入验证码！');
    } else if (val == num) {
        document.getElementById("normal").innerHTML = document.getElementById("fill").innerHTML;
        document.getElementById("fill").innerHTML = "";
        document.getElementById("fill").style = "width: 0%; height: 0%;background: transparent";
        document.getElementById("float-card").remove();

    } else {
        alert('验证码错误！');
        dj();
    }



}
function draw(show_num) {
    var canvas_width = document.getElementById('canvas').clientWidth;
    var canvas_height = document.getElementById('canvas').clientHeight;
    var canvas = document.getElementById("canvas");
    var context = canvas.getContext("2d");
    canvas.width = canvas_width;
    canvas.height = canvas_height;
    var sCode = "A,B,C,E,F,G,H,J,K,L,M,N,P,Q,R,S,T,W,X,Y,Z,1,2,3,4,5,6,7,8,9,0,q,w,e,r,t,y,u,i,o,p,a,s,d,f,g,h,j,k,l,z,x,c,v,b,n,m";
    var aCode = sCode.split(",");
    var aLength = aCode.length;

    for (var i = 0; i <= 3; i++) {
        var j = Math.floor(Math.random() * aLength);
        var deg = Math.random() * 30 * Math.PI / 180;
        var txt = aCode[j];
        show_num[i] = txt;
        var x = 10 + i * 20;
        var y = 20 + Math.random() * 8;
        context.font = "bold 23px ComicSans";
        context.translate(x, y);
        context.rotate(deg);

        context.fillStyle = randomColor();
        context.fillText(txt, 0, 0);

        context.rotate(-deg);
        context.translate(-x, -y);
    }
    for (var i = 0; i <= 5; i++) {
        context.strokeStyle = randomColor();
        context.beginPath();
        context.moveTo(Math.random() * canvas_width, Math.random() * canvas_height);
        context.lineTo(Math.random() * canvas_width, Math.random() * canvas_height);
        context.stroke();
    }
    for (var i = 0; i <= 30; i++) {
        context.strokeStyle = randomColor();
        context.beginPath();
        var x = Math.random() * canvas_width;
        var y = Math.random() * canvas_height;
        context.moveTo(x, y);
        context.lineTo(x + 1, y + 1);
        context.stroke();
    }
}
function randomColor() {
    let max = 220;
    let min = 0;
    var r = Math.floor(Math.random() * ((max - min) + min));
    var g = Math.floor(Math.random() * ((max - min) + min));
    var b = Math.floor(Math.random() * ((max - min) + min));
    return "rgb(" + r + "," + g + "," + b + ")";
}