
class FloatRoll {
    constructor(id, position = 0) {
        this.nAds = 1;
        let style = document.createElement('style');
        if (position) {
            style.innerText =
                `#adp_float_roll-${id} {
                width: 20%;
                min-width: 320px;
                height: 20%;
                min-height: 180px;
                bottom: 0px;
                left: 0px;
                position: fixed;
                z-index: 100001;
                border:none;
            }
            #adp_float_roll-${id}-iframe {
                width: 20%;
                min-width: 320px;
                height: 20%;
                min-height: 180px;
                bottom: 0px;
                left: 0px;
                position: fixed;
                z-index: 100001;
                border:none;
            }`;
        } else {
            style.innerText =
                `#adp_float_roll-${id} {
                width: 20%;
                min-width: 320px;
                height: 20%;
                min-height: 180px;
                bottom: 0px;
                right: 0px;
                position: fixed;
                z-index: 100001;
                border:none;
            }
            #adp_float_roll-${id}-iframe {
                width: 20%;
                min-width: 320px;
                height: 20%;
                min-height: 180px;
                bottom: 0px;
                right: 0px;
                position: fixed;
                z-index: 100001;
                border:none;
            }`;
        }

        document.getElementsByTagName('head')[0].appendChild(style);
        let area = document.getElementById('adp_float_roll-' + id);
        // area.setAttribute('style', 'background-color: #000;');
        let iframe = document.createElement('iframe');

        iframe.setAttribute('src', '//smotretonline.ru/video/f/rotator.php?id=' + id);
        iframe.setAttribute('allow', 'autoplay');
        iframe.setAttribute('id', `adp_float_roll-${id}-iframe`);
        area.appendChild(iframe);

        window.addEventListener('message', (event) => {
            switch (event.data) {
                case 'adp_VASTads_ended': {
                    console.log('next');
                    break;
                }

                case 'adp_ads_close':
                case 'adp_ads_ended': {
                    area.remove();
                    break;
                }
            }
        });


        setTimeout(this.drawCloseButton, 30 * 1000, area, position);
    }

    drawCloseButton(area, position) {
        let closeBtn = document.createElement('a');
        closeBtn.setAttribute('id', 'adp_close_button');
        closeBtn.innerText = 'Close';
        let style = document.createElement('style');
        if (position) {
            style.innerHTML = `
            #adp_close_button {
                position: absolute; 
                color: #fff; 
                background-color: #000; 
                left: 3px; 
                top: 3px; 
                z-index: 100002; 
                font-size: 12px; 
                cursor: pointer;
                border: none;
            }
        `;
        } else {
            style.innerHTML = `
            #adp_close_button {
                position: absolute; 
                color: #fff; 
                background-color: #000; 
                right: 3px; 
                top: 3px; 
                z-index: 100002; 
                font-size: 12px; 
                cursor: pointer;
                border: none;
            }
        `;
        }

        document.getElementsByTagName('head')[0].appendChild(style);
        area.appendChild(closeBtn);

        // closeBtn.onclick = () => {
        //     area.remove();
        // }
    }
}
