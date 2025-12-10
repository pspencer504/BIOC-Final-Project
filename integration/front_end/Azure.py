"""
Azure API code

Authors: Ian Ortega, Nabil Othman, Paige Spencer

Date: 12-9-2025

NOTE: Code adapted from https://learn.microsoft.com/en-us/azure/ai-services/speech-service/get-started-speech-to-text?tabs=new-foundry%2Cwindows&pivots=programming-language-python

"""

import os
import time
import azure.cognitiveservices.speech as speechsdk


def recognize_from_microphone(file):

    #get key and region as environment variables and create a speech object
    speech_config = speechsdk.SpeechConfig(subscription=os.environ.get('SPEECH_KEY'), region=os.environ.get('ENDPOINT'))
    speech_config.speech_recognition_language="en-US"

    #pass file name to speech recognizer
    audio_config = speechsdk.audio.AudioConfig(filename=file)
    speech_recognizer = speechsdk.SpeechRecognizer(speech_config=speech_config, audio_config=audio_config)

    speech_recognition_result = speech_recognizer.recognize_once_async().get()


    #process speech, catch errors, and print results
    if speech_recognition_result.reason == speechsdk.ResultReason.RecognizedSpeech:
        print("\nYou said: {}".format(speech_recognition_result.text))
        time.sleep(2)
        return speech_recognition_result.text
    elif speech_recognition_result.reason == speechsdk.ResultReason.NoMatch:
        print("No speech could be recognized: {}".format(speech_recognition_result.no_match_details))
    elif speech_recognition_result.reason == speechsdk.ResultReason.Canceled:
        cancellation_details = speech_recognition_result.cancellation_details
        print("Speech Recognition canceled: {}".format(cancellation_details.reason))
        if cancellation_details.reason == speechsdk.CancellationReason.Error:
            print("Error details: {}".format(cancellation_details.error_details))
            print("Did you set the speech resource key and endpoint values?")
