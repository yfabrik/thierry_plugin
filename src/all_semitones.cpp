#include "plugin.hpp"

struct All_semitones : Module
{
	enum ParamId
	{
		OCTAVE_PARAM,
		PITCH_PARAM,
		PARAMS_LEN
	};
	enum InputId
	{
		SIGNAL_IN_INPUT,
		INPUTS_LEN
	};
	enum OutputId
	{
		C4_OUTPUT_OUTPUT,
		SEMITONE_OUTPUT_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId
	{
		LIGHTS_LEN
	};

	All_semitones()
	{
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

		configParam(OCTAVE_PARAM, -5.f, 5.f, 0.f, "octaves");
		paramQuantities[OCTAVE_PARAM]->snapEnabled = true;
		configParam(PITCH_PARAM, -12.f, 12.f, 0.f, "semitone");
		paramQuantities[PITCH_PARAM]->snapEnabled = true;

		configInput(SIGNAL_IN_INPUT, "");
		configOutput(C4_OUTPUT_OUTPUT, "");
		configOutput(SEMITONE_OUTPUT_OUTPUT, "");
	}

	float phase = 0.f;
	float phase2 = 0.f;
	float phase3 = 0.f;
	float phase4 = 0.f;

	void process(const ProcessArgs &args) override
	{
		float *inputBuffer = inputs[SIGNAL_IN_INPUT].getVoltages();
		float *sortie;


		/******HARMONIQUE*******/
		// Generate a complex waveform with harmonics
		float fundamentalFrequency = 440.0; // Set the fundamental frequency (in Hz)
		float amplitude = 5.0;				// Set the amplitude of the fundamental

		phase += fundamentalFrequency * args.sampleTime;
		if (phase >= 1.f)
			phase -= 1.f;
		// phase2 += 2 * fundamentalFrequency * args.sampleTime;
		// if (phase2 >= 1.f)
		// 	phase2 -= 1.f;

		// phase3 += 3 * fundamentalFrequency * args.sampleTime;
		// if (phase3 >= 1.f)
		// 	phase3 -= 1.f;

		float outputSignal = amplitude * std::sin(2.0 * M_PI * phase);

		// Add harmonics (adjust the frequencies and amplitudes as needed)
		// outputSignal += 0.5 * amplitude * std::sin(2.0 * M_PI * phase2); // Second harmonic
		// outputSignal += 0.2 * amplitude * std::sin(2.0 * M_PI * phase3); // Third harmonic

		/*********************/
		// SineOscillator _sine;

		float input_signal = inputs[SIGNAL_IN_INPUT].isConnected() ? inputs[SIGNAL_IN_INPUT].getVoltage() : outputSignal;

		float fundamentale2 = inputs[SIGNAL_IN_INPUT].isConnected() ? input_signal : fundamentalFrequency;

		float pitch = params[PITCH_PARAM].getValue();
		float octave = params[OCTAVE_PARAM].getValue();

		float freq2 = fundamentale2 * std::pow(2.f, pitch / 12.f) * std::pow(2.f, octave);

		phase4 += freq2 * args.sampleTime;
		if (phase4 >= 1.f)
			phase4 -= 1.f;

		float sine2 = std::sin(2.f * M_PI * phase4);

		outputs[C4_OUTPUT_OUTPUT].setVoltage(input_signal);

		// _sine.setFrequency(440);

		outputs[SEMITONE_OUTPUT_OUTPUT].setVoltage(sine2);
	}
};

struct All_semitonesWidget : ModuleWidget
{
	All_semitonesWidget(All_semitones *module)
	{
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/all_semitones.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.227, 42.18)), module, All_semitones::OCTAVE_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.227, 60.17)), module, All_semitones::PITCH_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.397, 79.125)), module, All_semitones::SIGNAL_IN_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.079, 99.899)), module, All_semitones::C4_OUTPUT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(14.845, 117.826)), module, All_semitones::SEMITONE_OUTPUT_OUTPUT));
	}
};

Model *modelAll_semitones = createModel<All_semitones, All_semitonesWidget>("all_semitones");