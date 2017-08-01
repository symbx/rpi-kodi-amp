import xbmc, xbmcplugin, xbmcaddon, xbmcgui, sys, socket, os

class Amplifier:
	def __init__(self):
		self.id = 'script.amplifier'
		self.addon = xbmcaddon.Addon(self.id)
		self.sock = None

	def run(self):
		self.connect()
		while True:
			idx = xbmcgui.Dialog().select('Amplifier', ['Initialize', 'Master volume', 'Volume trims', 'Tones', 'Functions', 'Input line', 'Mutes'])
			if idx == 0:
				self.initAmplifier()
			elif idx == 1:
				self.masterVolume()
			elif idx == 2:
				self.trims()
			elif idx == 3:
				self.tones()
			elif idx == 4:
				self.functions()
			elif idx == 5:
				self.input()
			elif idx == 6:
				self.mutes()
			else:
				break;

	def connect(self):
		if not os.path.exists('/tmp/amplifier.sock'):
			self.fail()
			return
		if not self.sock is None:
			return
		self.sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
		self.sock.connect('/tmp/amplifier.sock')

	def send(self, cmd, val):
		if self.sock is None:
			self.fail()
			return
		req = bytearray()
		req.append(cmd)
		req.append(val)
		self.sock.send(req)

	def read(self):
		res = self.sock.recv(1)
		print('kodi receive '+str(res))
		return ord(res)

	def fail(self):
		xbmc.executebuiltin('XBMC.Notification(%s,%s, %s)' % ('Error', 'Can\'t connect to amplifier!', str(10 * 1000)))

	def initAmplifier(self):
		self.send(CMD_INIT, 0)

	def masterVolume(self):
		self.send(CMD_GET_VOL, 0)
		vol = self.read()
		vol = xbmcgui.Dialog().input('Master volume', str(vol), xbmcgui.INPUT_NUMERIC)
		if vol == '':
			return
		if vol > 60:
			vol = 60
		if vol < 0:
			vol = 0
		self.send(CMD_SET_VOL, vol)

	def trims(self):
		idx = xbmcgui.Dialog().select('Channel', ['Front left', 'Front right', 'Center', 'Rear reft', 'Rear right', 'Subwoofer'])
		if idx < 0:
			return
		idx += 1
		idx = idx << 4
		self.send(CMD_GET_TRIM, idx)
		trim = self.read()
		trim = xbmcgui.Dialog().input('Trim volume', str(trim), xbmcgui.INPUT_NUMERIC)
		if trim == '':
			return
		if trim > 15:
			trim = 15
		if trim < 0:
			trim = 0
		self.send(CMD_SET_TRIM, idx | trim)

	def tones(self):
		idx = xbmcgui.Dialog().select('Type', ['Base', 'Middle', 'Treble'])
		if idx < 0:
			return
		idx += 9
		idx = idx << 4
		self.send(CMD_GET_TONE, idx)
		tone = self.read()
		tone = xbmcgui.Dialog().input('Trim volume', str(trim), xbmcgui.INPUT_NUMERIC)
		if tone == '':
			return
		if tone > 15:
			tone = 15
		if tone < 0:
			tone = 0
		self.send(CMD_SET_TRIM, idx | tone)

	def confirm(self, title, text):
		return xbmcgui.Dialog().yesno(title, text)

	def functions(self):
		idx = xbmcgui.Dialog().select('Function', ['3D', 'Enchanced Surround', 'Mix channels', 'Mute', 'Tone defeat'])
		if idx == 0:
			self.send(CMD_F_3D, 1 if self.confirm('Enable function?', '3D') else 0)
		elif idx == 1:
			self.send(CMD_F_SURROUND, 1 if self.confirm('Enable function?', 'Enchanded Surround') else 0)
		elif idx == 1:
			self.send(CMD_F_MIX, 1 if self.confirm('Enable function?', 'Mix channels') else 0)
		elif idx == 3:
			self.send(CMD_F_MUTE, 1 if self.confirm('Enable function?', 'Mute') else 0)
		elif idx == 4:
			self.send(CMD_F_TONE, 1 if self.confirm('Enable function?', 'Tone defeat') else 0)

	def input(self):
		idx = xbmcgui.Dialog().select('Input line', ['Line 1', 'Line 2', 'Line 3', 'Line 4', 'Direct (6 channels)'])
		if idx < 0:
			return
		if idx == 4:
			idx = 7
		else:
			idx += 7
		self.send(CMD_INPUT, idx)

	def mutes(self):
		channels = ['Front left', 'Front right', 'Center', 'Subwoofer', 'Rear reft', 'Rear right', 'All']
		idx = xbmcgui.Dialog().select('Channel', channels)
		if idx < 0:
			return
		mute = self.confirm('Mute?', channels[idx])
		idx *= 2
		idx = idx << 4
		if mute:
			idx += 1
		self.send(CMD_MUTE, idx)


CMD_SET_VOL = 1
CMD_GET_VOL = 2
CMD_SET_TRIM = 3
CMD_GET_TRIM = 4
CMD_SET_TONE = 5
CMD_GET_TONE = 6
CMD_F_TONE = 7
CMD_F_3D = 8
CMD_F_MUTE = 9
CMD_F_SURROUND = 11
CMD_F_MIX = 12
CMD_INPUT = 10
CMD_MUTE = 13
CMD_INIT = 127

		
if __name__ == '__main__':
	app = Amplifier()
	app.run()
	del app
