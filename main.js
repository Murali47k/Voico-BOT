const inst = document.getElementById('instruction');
        const status = document.getElementById('status');
        let isListening = false;

        window.SpeechRecognition = window.SpeechRecognition || window.webkitSpeechRecognition;
        const recognition = new SpeechRecognition();
        recognition.interimResults = false; // We only care about final results

        // Valid instructions
        const validInstructions = ["left", "right", "forward", "backward"];

        // Start recognition on click
        function toggleRecognition() {
            if (!isListening) {
                recognition.start();
                status.innerText = "Listening";
                isListening = true;
            } else {
                recognition.stop();
                status.innerText = "Not Listening";
                isListening = false;
            }
        }

        recognition.addEventListener('result', (e) => {
            const text = Array.from(e.results)
                .map(result => result[0])
                .map(result => result.transcript.toLowerCase())
                .join('');

            if (validInstructions.some(instr => text.includes(instr))) {
                const detectedInstr = validInstructions.find(instr => text.includes(instr));
                const finaltext=detectedInstr.charAt(0).toUpperCase() + detectedInstr.slice(1);
                inst.innerText = `Moving ${finaltext}`;
            } else {
                inst.innerText = "Invalid Instruction";
            }
        });

        recognition.addEventListener('end', () => {
            if (isListening) recognition.start(); // Restart recognition if it should still be listening
        });